// Logan Jenny Autumn 2018
#include <stdio.h>

void produceImages(FILE *infile1, FILE *infile2);
void checkerBoard(unsigned char image1[], unsigned char image2[], int size, int width, unsigned char header[]);
void doubleExposure(unsigned char image1[], unsigned char image2[], int size, unsigned char header[]);

int main(void) {
    FILE *infile1 = fopen("in1.bmp", "rb");
    FILE *infile2 = fopen("in2.bmp", "rb");
    produceImages(infile1, infile2);
    return 0;
}

//read files and use helpermethods to produce checkerboard and doubleexposure
void produceImages(FILE *infile1, FILE *infile2) {
    int size, width, height;
    unsigned char header1[54];
    unsigned char header2[54];

    //read headers - pull size, height, and width
    fread( header1, 1 , 18 , infile1);
    fread( &width, sizeof(int) , 1,  infile1);
    fread( &height, sizeof(int) , 1,  infile1);
    fread( header1, 1 , 8 , infile1);
    fread( &size, sizeof(int) , 1,  infile1);
    fread( header1, 1 , 16 , infile1);
    fread( header2, 1 , 54 , infile2);

    //read image
    unsigned char image1[height*width*3];
    unsigned char image2[height*width*3];
    fread(image1, 1, size, infile1);
    fread(image2, 1, size, infile2);

    //close files
    fclose(infile1);
    fclose(infile2);

    //produce output
    doubleExposure(image1, image2, size, header2);
    checkerBoard(image1, image2, size, width, header2);
}

//produces checkerboard bmp image
void checkerBoard(unsigned char image1[], unsigned char image2[], int size, int width, unsigned char header[]) {
    unsigned char cb[size+54];

    //load header data
    for (int i = 0; i < 54; i++) {
        cb[i] = header[i];
    }

    //for loops control number of iterations, count controls index
    int count = 0;
    for (int i = 0;  i < 8;  i++ ) {
        for (int j = 0;  j < width;  j++) {
            if ( (i % 2) == (j % 2) ) {
                for (int n = 0; n < (width/8)*3; n++) {
                    cb[count+54] = image1[count];
                    count++;
                }
            } else {
                for (int n = 0; n < (width/8)*3; n++) {
                    cb[count+54] = image2[count];
                    count++;
                }
            }
        }
    }

    //output
    FILE *ofile1 = fopen("cb.bmp", "wb");
    fwrite(cb, sizeof(unsigned char), size+54, ofile1);
    fclose(ofile1);
}

//produces double exposure bmp image
void doubleExposure(unsigned char image1[], unsigned char image2[], int size, unsigned char header[]) {
    unsigned char de[size+54];

    //read header data
    for (int i = 0; i < 54; i++) {
        de[i] = header[i];
    }

    //produce averages
    for (int i = 0; i < size; i++) {
        de[i+54] = (image1[i] + image2[i]) / 2;
    }

    //output
    FILE *ofile2 = fopen("de.bmp", "wb");
    fwrite(de, sizeof(unsigned char), size+54, ofile2);
    fclose(ofile2);
}
