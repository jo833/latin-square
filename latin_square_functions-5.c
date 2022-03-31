/******************************************************************************
 * @file: latin_square_functions.c
 * @creator: Julia Oghigian (oghigian@wisc.edu)
 *****************************************************************************/

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#include "latin_square_functions.h"

// Set this to 1 to enable dbgprintf statements, make sure to set it back to 0 
// before submitting!
#define DEBUG               0 
#define dbgprintf(...)      if (DEBUG) { printf(__VA_ARGS__); }
#define dbgprint_latin_square(n, LS) if (DEBUG) { Print_Latin_Square(n, LS); }

/******************************************************************************
 * Data Types and Structures
 *****************************************************************************/

// ADD ANY ADDITIONAL DATA TYPES OR STRUCTSvHERE 

/******************************************************************************
 * Globals
 *****************************************************************************/
extern void Print_Latin_Square(const size_t n, char **latin_square);

/******************************************************************************
 * Helper functions
 *****************************************************************************/

// ADD ANY HELPER FUNCTIONS YOU MIGHT WRITE HERE 

/******************************************************************************
 * Verificaiton functions
 *****************************************************************************/

/* 
 * This function takes the name of the file containing the latin square
 * and reads in the data to the the latin_square parameter.  
 *
 * There are many approaches that will work to read in the latin square data.
 * In any approach you choose, you will need to do at least the following:
 *     1) open the file 
 *     2) read in the text from the file
 *     3) figure out the dimensions of the latin square (n)
 *     4) reserve memory for the latin_square. This requires 2 steps
 *         4a) reserve an array of pointers to the rows
 *         4b) reserve an array of characters for each row
 *     5) fill in the latin_square data structure 
 *     6) close the file
 *
 * @param filename The name of the file to read in
 * @param latin_square_in A pointer to the latin square variable in main
 * @param n The value of both dimensions of the latin square (i.e. nxn)
 */
void Read_Latin_Square_File(const char *filename, 
                            char ***latin_square_in, 
                            size_t *n) {
    /* BEGIN MODIFYING CODE HERE */
    
    
    FILE *f;
    f = fopen(filename,"r" );
    char c;


    *n = 0;
     while((c = fgetc(f)) != EOF){
           if( c =='\n'){
              *n = *n +1;
          }
     }
    *n = *n - 1; 
    fclose(f);
    //printf("%d\n",*n);
    FILE *fp;
    fp = fopen(filename, "r");

    *latin_square_in = (char **)malloc(*n*sizeof(char*));

    for(int i = 0; i < *n; i++){
        *((*latin_square_in) + i)  = (char *)(malloc((*n+2)*sizeof(char)));
    }

    for(int i = 0; i < *n; i++){
        fgets(*((*latin_square_in) + i ), *n+2, (FILE*)fp);
    }

    fclose(fp);
//   free(**latin_square_in);
 /* END MODIFYING CODE HERE */
}

/** 
 * This function checks to see that exactly n symbols are used and that 
 * each symbol is used exactly n times.
 *
 * @param n The value of both dimensions of the latin square (i.e. nxn)
 * @param latin_square_in A pointer to the latin square variable in main
 * @return 1 if valid, 0 if not
 */
int Verify_Alphabet(const size_t n, char **latin_square) {
    if (latin_square == NULL) {
        printf("Verify_Alphabet - latin_square is NULL\n"); 
        return 0;
    }

int *counter =calloc(256, sizeof(int));

    for (int j = 0; j < n; j++){
         char *row =  *(latin_square + j);     
        for (int k = 0; k <n; k++){ 
             int l = 0;
             for (unsigned char  i = 0; i <= 255; i++){
                  if (*(row + k) == i){
                      *(counter+ l) += 1;
                  }
                  if (i == 255){
                      break;
                  }
                  l++;
             }
         }
    }

    for (int i = 0; i < 256; i++){
         if(*(counter+i) != 0){
            if(*(counter+i) != n){ //unter[i] > n || counter[i] < n){
               free(counter);
               return 0;
            }
         }
    }
   free(counter);
    
    return 1;
}

/* 
 * This function verifies that no symbol is used twice in a row or column.
 * It prints an error message alerting the user which rows or columns have 
 * duplicate symbols. This means that you will have at most n row error prints
 * and n column error prints.
 * 
 * Note: Test all rows first then test all columns.
 * 
 * Error messages have been included for you. Do not change the format out the 
 * print statements, as our grading scripts will use exact string matching. You
 * change the variable passed as the formatting argument to printf.
 * 
 * @param n The value of both dimensions of the latin square (i.e. nxn)
 * @param latin_square_in A pointer to the latin square variable in main
 * @return 1 if valid, 0 if not
 */
 int Verify_Rows_and_Columns(const size_t n, char **latin_square){
    if (latin_square == NULL) {
        printf("Verify_Rows_and_Columns - latin_square is NULL\n"); 
        return 0;
    }
 int boolean = 0; 
 int *counter = calloc(n, sizeof(int));
     for (int j = 0; j < n; j++){
          char *row =  *(latin_square + j);
          for (int k = 0; k <n; k++){
             for(int l = k + 1; l < n; l++){
                 if(*(row + k) == *(row + l)){
                      *(counter + j) += 1;
                    }             
                 }
              }     
          }
  int *counterCol = calloc(n, sizeof(int)); 
  
    for (int c = 0; c < n; c++) { 
			for (int r = 0; r < n; r++) {
				for (int step = r + 1; step < n; step++) {
                     if (*((*(latin_square + r)) + c) == *((*(latin_square +step)) + c)){
                       *(counterCol +c ) += 1;
                     }

				}
			}
			
		}
     for (int i = 0; i < n; i++){
          if(*(counter + i ) > 0){
             printf("Error in row %d\n", i);
             boolean++;
          }
     }
     for (int i = 0; i < n; i++){
           if(*(counterCol + i ) > 0){
              printf("Error in column %d\n", i);
              boolean++;
           }  
      } 
 free(counter);
  free(counterCol);   
     if(boolean > 0){
        return 0;
     } 
   return 1;
}

/* 
 * This function calls free to allow all memory used by the latin_square 
 * verification program to be reclaimed.
 *
 * Note: you will have n+1 calls to free
 *
 * @param n The value of both dimensions of the latin square (i.e. nxn)
 * @param latin_square_in A pointer to the latin square variable in main
 */ 
void Free_Memory(const size_t n, char **latin_square) {
  
    if (latin_square == NULL){
        ;
    } else{
     for (int i =0; i < n; i++){
  free(  *((latin_square) + i ));
 }

        free(latin_square);
    }
}


