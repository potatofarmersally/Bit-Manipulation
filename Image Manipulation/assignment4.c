/*
 * assignment4.c
 *
 *  Created on: Apr 27, 2018
 *      Author: Sally Ho
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
/* PROTOTYPE: this gets the file size. */
unsigned long get_file_size(char *);
/* PROTOTYPE: this blends the two files together*/
void blendFunction(unsigned char*, unsigned char*, unsigned long);
/* PROTOTYPE: this makes the file into checkers.*/
void checkerFunction(unsigned char*, unsigned char*, unsigned long);
/*
 * Main function to execute everything.
 */
int main(int argc, char **argv) {
	FILE *input1, *input2;
	char *fileName1, *fileName2; //
	unsigned long file1Size, file2Size;
	unsigned char *image1, *image2, *bm1, *bm2; //for malloc
	int image1Row, image1Col, image2Row, image2Col;
	fileName1 = *++argv; //getting two file names
	fileName2 = *++argv;
	file1Size = get_file_size(fileName1);//file sizes for
	file2Size = get_file_size(fileName2);//the 2 files. (uns long)
	image1 = (unsigned char *)malloc(file1Size);
	image2 = (unsigned char *)malloc(file2Size);
	input1 = fopen(fileName1, "rb");//open all the files.
	input2 = fopen(fileName2, "rb");
	fread(image1, 1, file1Size, input1);
	fread(image2, 1, file2Size, input2);
	if((file1Size >= 0 && file2Size >= 0) //check if file exists
			&& (file1Size == file2Size)) {
		image1Row = *((int *) (image1 + 18)); //width
		image1Col = *((int *) (image1 + 22)); //height
		image2Row = *((int *) (image2 + 18)); //width
		image2Col = *((int *) (image2 + 22)); //height
		if(image1Row == image1Col && image2Row == image2Col) {
			bm1 = image1;
			bm2 = image2;
			if(*bm1 == 'B' && *bm2 == 'B' //check if bitmap files
					&& *++bm1 == 'M' && *++bm2 == 'M') {
				blendFunction(image1, image2, file1Size);
				checkerFunction(image1, image2, file1Size);
			} else {
				printf("NOT A BMP");
			}
		} else {
			printf("Aren't squares!");
		}
	} else {
		printf("FILES AREN'T SAME SIZE.");
	}
	fclose(input1);
	fclose(input2);
	free(image1);
	free(image2);
	image1 = NULL;
	image2 = NULL;
	return 0;
}
/*
 * Function to get the file size after passing in a file name
 * returns an unsigned long containing the size of the file.
 */
unsigned long get_file_size(char *s) {
	long result = -1;
	struct stat file_info;
	if(stat(s, &file_info) > -1) {
		result = (unsigned long)(file_info.st_size);
	}
	return (unsigned long)result;
}
/*
 * This function blends 2 images together and writes them to one
 * file.
 */
void blendFunction(unsigned char* image1,
		unsigned char *image2, unsigned long fileSize) {
	FILE *blend = fopen("blend.bmp", "wb");
	int i, j;
	unsigned char *result, *memory;
	memory = (unsigned char *)malloc(fileSize);
	result = memory;
	for(i = 0; i < 54; i++) { //for the headers.
		*result++ = *image1++;
		*image2++;
	}
	for(j = 54; j < fileSize; j++) {
		*result++ = (*image1++ + *image2++) / 2;
	}
	fwrite(memory, sizeof(unsigned char), fileSize, blend);
	fclose(blend);
	free(memory);
	memory = NULL;
}
/*
 * This function takes 2 images together and checkers them.
 */
void checkerFunction(unsigned char *image1,
		unsigned char *image2, unsigned long fileSize) {
	FILE *checkered = fopen("checker.bmp", "wb");
	int i, j, k;
	int swap = 1;
	int flip = 1;
	//because the 18th and 22nd byte contains the width and height.
	int rows = *((int *) (image1 + 18)); //width
	int columns = *((int *) (image1 + 22)) * 3; //height
	unsigned char *result, *memory;
	memory = (unsigned char *)malloc(fileSize);
	result = memory;
	for(i = 0; i < 54; i++) {
		*result++ = *image1++;
		*image2++;
	}
	for(j = 0; j < rows; j++) {
		for(k = 0; k < columns; k++) {
			if(flip % 2 == 0) {
				if(swap % 2 == 1) {
					*result++ = *image2++;
					*image1++; //remember to increment through
				} else {
					*result++ = *image1++;
					*image2++; //each alternate image
				}
			} else {
				if(swap % 2 == 1) {
					*result++ = *image1++;
					*image2++; //so it can keep up
				} else {
					*result++ = *image2++;
					*image1++;
				}
			}
			if((k % (columns / 8)) == 0) {
				swap++;
			}
		}
		if((j % (rows / 8)) == 0) {
			flip++;
		}
	}
	fwrite(memory, sizeof(unsigned char), fileSize, checkered);
	fclose(checkered);
	free(memory);
	memory = NULL;
}
