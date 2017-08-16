//
//  main.c
//  Gradient_Descent
//
//  Created by Henry Boswell on 8/6/17.
//  Copyright © 2017 Henry Boswell. All rights reserved.
// Inspired by Andrew Ng's course on machine learning



#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#define BUFF 65536

char buffer [BUFF];

void printMAT(long double *MAT,int N){
    
    printf("N = %d \n", N);
    for (int i = 0; i <  4; i++){
        printf("\n");
        for (int j = 0; j < 4; j++){
            printf(" %Lf ",*(MAT + i*N + j));
            
        }
    }
}
void naive_IKJ_Square(long double *MAT1, long double *MAT2, long double*MAT4, int N){
    
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            for (int k = 0; k < N; k++) {
                // MAT3[i][j] += MAT1[i][k] * MAT2[k][j];
                *(MAT4 + i*N + j) += *(MAT1 + i*N + k) * *(MAT2 + k*N + j);
            }
        }
    }
    
    
}

void multiply_By_Transpose(long double *MAT1, long double *MAT2, long double*MAT4, int N){
    
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            for (int k = 0; k < N; k++) {
                // MAT3[i][j] += MAT1[i][k] * MAT2[k][j]; standard mult
                *(MAT4 + i*N + j) += *(MAT1 + i*N + k) * *(MAT2 + k + j*N);
            }
        }
    }
    
    
}
void subtract_y_in_place(long double *MAT1, long double *MAT2, int N){
    
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < 1; j++) {
            // MAT3[i][j] += MAT1[i][k] * MAT2[k][j];
            *(MAT1 + i*N + j) = *(MAT1 + i*N + j) - *(MAT2 + i*N + j);
        }
        
    }
}
void square_in_place(long double *MAT1, int N){
    
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < 1; j++) {
            // MAT3[i][j] += MAT1[i][k] * MAT2[k][j];
            *(MAT1 + i*N + j) = *(MAT1 + i*N + j) * *(MAT1 + i*N + j);
        }
    }
}

long double sum(long double *MAT1, int N){
    long double sum = 0;
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < 1; j++) {
            // MAT3[i][j] += MAT1[i][k] * MAT2[k][j];
            sum = sum + *(MAT1 + i*N + j);
        }
    }
    return sum;
}

void single_mult_vector(long double* MAT1, long double* MAT2, long double* MAT3, int col, int N){
    
    for (int i = 0; i < N; i++) {
        //2*2 = 4
        //3*3 = 9
        //5*5 = 25
        *(MAT3 + i*N) = *(MAT1 + i*N) * *(MAT2 + i*N + col);
        
    }
    
}

long double computeCost(long double* X, long double* yVEC,long double* theta, int N){
    long double *inner = (long double *)calloc(N * N, sizeof(long double));
    multiply_By_Transpose(X, theta, inner, N);
    //printMAT(inner,N);
    subtract_y_in_place(inner, yVEC, N);
    //printMAT(inner,N);
    square_in_place(inner, N);
    //printMAT(inner,N);
    long double summ = 0;
    summ = sum(inner,N);
    summ =summ/(2*N);
    printf("\n Cost =  %Lf", summ);
    return summ;
    
    
}


long double* gradient_descent(long double* X, long double* y, long double*theta, long double alpha, int epoch, int N, int params){
    long double *thetaNew = (long double *)calloc(N * N, sizeof(long double));
   // long double *cost = (long double *)malloc(epoch * sizeof(long double));
    
    long double *errorBig = (long double *)calloc(N * N, sizeof(long double));
    
    
    //long double *error = (long double *)calloc(params, sizeof(long double));
    
    for(int i = 0; i < epoch; i++){
        multiply_By_Transpose(X, theta, errorBig, N);
        subtract_y_in_place(errorBig, y, N);
        //printMAT(errorBig,N);
        
        for(int j = 0; j < params; j++){
            long double *thetaTerm = (long double *)calloc(N * N, sizeof(long double));
            //void single_mult_vector(long double* MAT1, long double* MAT2, long double* MAT3, int col, int N){
            single_mult_vector(errorBig,X,thetaTerm,j,N);
            long double sum2 = sum(thetaTerm, N);
            *(thetaNew + j) = *(theta + j*N) - ((alpha/N)*sum2);
            free(thetaTerm);
            
            

            
            
        }
        theta = thetaNew;
        
        //printMAT(theta, N);
       computeCost(X, y, theta,N);
       // printf(" \n cost \n %Lf", c);
    }
    return theta;
    
    
}
/*
void testSingle(){
    int N = 97;
    
    long double *X = (long double *)calloc(N * N, sizeof(long double));
    long double *XT = (long double *)calloc(N * N, sizeof(long double));
    long double *yVEC = (long double *)calloc(N * N, sizeof(long double));
    long double *Theta = (long double *)calloc(N * N, sizeof(long double));
    int i = 0;
    int j = 0;
    FILE *fp = fopen("ex1data1.txt", "r");
    if(fp != NULL){
        while(fgets(buffer, 300, (FILE*) fp)) {
            
            printf("%s\n", buffer);
            i = 0;
            
            
            *(XT + i*N + j) = 1;
            *(X + j*N + i) = 1;
            int counter = 1;
            for (char *p = strtok(buffer,","); p != NULL; p = strtok(NULL, ","))
            {
                if(counter == 1 || counter == 2){
                    if(counter == 2){
                        long double f;
                        f = atof(p);
                        *(yVEC + j*N) = f;
                    }
                    
                    else{
                        long double f;
                        f = atof(p);
                        *(XT + i*N + j+N) = f;
                        *(X + j*N + i +1) = f;
                        
                        printf("token: %s\n", p);
                    }
                    
                }
                i++;
                counter++;
            }
            j++;
        }
        fclose(fp);
        //  printMAT(XT,N);
        //  printMAT(yVEC ,N);
        printf("here");
        computeCost(X, yVEC, Theta,N);
        //void gradient_descent(long double* X, long double* y, long double*theta, int alpha, int epoch, int N, int params){
        gradient_descent(X,yVEC,Theta,300.0,1000,N,2);
        //naive_IKJ_Square(X,XT,inner, N);
        
        
        //printf("%d",j);
        //printMAT(inner,N);
        
    }

}
 */

long double findMin(long double* MAT, int N, int col){
    long double min = 0;
    for (int i = 0; i < N; i++) {
        //2*2 = 4
        //3*3 = 9
        //5*5 = 25
        if(*(MAT + i*N + col) < min){
            min = *(MAT + i*N + col);
        }
        
        
    }

    return min;
}
long double findMax(long double* MAT, int N, int col){
    long double max = 0;
    for (int i = 0; i < N; i++) {
        //2*2 = 4
        //3*3 = 9
        //5*5 = 25
        if(*(MAT + i*N + col) > max){
            max = *(MAT + i*N + col);
        }
        
        
    }
    
    return max;
}

void normalize(long double* MAT, int N, int cols){
    for(int j = 0; j < cols; j ++){
        long double min = findMin(MAT,N,j);
        long double max = findMax(MAT,N,j);
        
        for (int i = 0; i < N; i++) {
            //2*2 = 4
            //3*3 = 9
            //5*5 = 25
            *(MAT + i*N + j) = ((*(MAT + i*N + j) -min)/(max-min));
        }
    }
}

int main(int argc, char *argv[])
{
    
    int N = 47;
    
    long double *X = (long double *)calloc(N * N, sizeof(long double));
    long double *XT = (long double *)calloc(N * N, sizeof(long double));
    long double *yVEC = (long double *)calloc(N * N, sizeof(long double));
    
    
    long double *Theta = (long double *)calloc(N * N, sizeof(long double));
    
    //int actual = 97;
    
    
    int i = 0;
    int j = 0;
    FILE *fp = fopen("ex1data2.txt", "r");
    if(fp != NULL){
        while(fgets(buffer, 300, (FILE*) fp)) {
            
            printf("%s\n", buffer);
            i = 0;
            
            
            *(XT + i*N + j) = 1;
            *(X + j*N + i) = 1;
            
            
            
            int counter = 1;
            for (char *p = strtok(buffer,","); p != NULL; p = strtok(NULL, ","))
            {
                
                    if(counter == 3){
                        long double f;
                        f = (long double)atof(p);
                        *(yVEC + j*N) = f;
                    }else{
                    
               
                    long double f;
                    f = (long double)atof(p);
                    *(XT + i*N + j+N) = f;
                    *(X + j*N + i +1) = f;
                    
                    printf("token: %s\n", p);
                    }
                
                    
            
                i++;
                counter++;
                
            }
            j++;
            
            
            
        }
        fclose(fp);
        normalize(X, N, 3);
        normalize(yVEC, N, 1);
        printMAT(X,N);
        printMAT(yVEC ,N);
        printf("here");
        computeCost(X, yVEC, Theta,N);
        //void gradient_descent(long double* X, long double* y, long double*theta, int alpha, int epoch, int N, int params){
        Theta = gradient_descent(X,yVEC,Theta,0.001,10000,N,3);
        //naive_IKJ_Square(X,XT,inner, N);
        printMAT(X,N);
        printMAT(yVEC ,N);
        //printMAT(theta ,N);
        
        long double test =  (*(Theta))*(*(X+N))+(*(Theta+1))*(*(X+1+N))+(*(Theta+2))*(*(X+2+N));
        printf("\n %Lf \n", test);
        printf("\n %Lf \n", (*(yVEC + N)));
        
        //printf("%d",j);
        //printMAT(inner,N);
        
    }
}

