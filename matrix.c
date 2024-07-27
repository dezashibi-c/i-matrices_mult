// ***************************************************************************************
//    Project: Matrices Multiplier in C
//    File: matrix.c
//    Date: 2024-07-27
//    Author: Navid Dezashibi
//    Contact: navid@dezashibi.com
//    Website: https://www.dezashibi.com | https://github.com/dezashibi
//    License:
//     Please refer to the LICENSE file, repository or website for more information about
//     the licensing of this work. If you have any questions or concerns,
//     please feel free to contact me at the email address provided above.
// ***************************************************************************************
// *  Description:
// ***************************************************************************************

#include <stdio.h>
#include <stdlib.h>

/**
 * @brief Function to allocate memory for a matrix
 *
 * it allocates a 2-Dimensional array or better to say
 * a pointer-to-pointer based on number of given rows and columns
 *
 * @param rows number of matrix rows
 * @param cols number of matrix columns
 *
 * @return a pointer-to-pointer for the matrix
 *
 * `NOTE: it allocates memory`
 */
int** allocate_matrix(int rows, int cols)
{
    int** matrix = (int**)malloc(rows * sizeof(int*));

    if (!matrix)
    {
        perror("Failed to allocate memory for matrix rows");
        exit(EXIT_FAILURE);
    }

    for (int i = 0; i < rows; i++)
    {
        matrix[i] = (int*)malloc(cols * sizeof(int));
        if (!matrix[i])
        {
            perror("Failed to allocate memory for matrix columns");
            exit(EXIT_FAILURE);
        }
    }

    return matrix;
}

/**
 * @brief Function to free the allocated memory for a matrix
 *
 * use it when you're done with the matrix
 *
 * @param matrix a pointer-to-pointer to the integer matrix
 * @param rows number of the rows the matrix has
 */
void free_matrix(int** matrix, int rows)
{
    // first free the columns
    for (int i = 0; i < rows; i++)
    {
        // each row has multiple column (an array of integer value so to say)
        free(matrix[i]);
    }

    // then free the matrix itself
    free(matrix);
}

/**
 * @brief Function to load a matrix from a file
 *
 * it checks for the validity of data, the data must be properly formatted
 * no trailing space, no illegal characters, only numbers
 *
 * First line is the size of the first matrix
 * Then the first matrix data
 * Then comes the second matrix size
 * And finally the second matrix data
 *
 * An example:
 * ```
 * 2 3
 * 1 2 9
 * 4 5 6
 * 3 2
 * 7 8
 * 9 10
 * 11 12
 * ```
 *
 * @param file pointer to the file containing the matrix data
 * @param rows a pointer to the matrix's rows
 * @param cols a pointer to the matrix's columns
 *
 * @return a pointer-to-pointer to the extracted matrix data from file
 *
 * `NOTE: it allocates memory`
 */
int** load_matrix(FILE* file, int* rows, int* cols)
{
    if (fscanf(file, "%d %d", rows, cols) != 2)
    {
        fprintf(stderr, "Failed to read matrix dimensions\n");
        exit(EXIT_FAILURE);
    }

    // Allocate memory to the matrix pointer to be loaded with the data from the file
    int** matrix = allocate_matrix(*rows, *cols);

    for (int i = 0; i < *rows; i++)
    {
        for (int j = 0; j < *cols; j++)
        {
            if (fscanf(file, "%d", &matrix[i][j]) != 1)
            {
                fprintf(stderr, "Failed to read matrix values\n");
                exit(EXIT_FAILURE);
            }
        }
    }

    return matrix;
}

/**
 * @brief Function to multiply two matrices
 *
 * It also checks the possibility of multiplication
 * First matrix's number of columns must be equal to second matrix's number of rows
 *
 * @param a a pointer-to-pointer to the first matrix
 * @param a_rows number of first matrix's rows
 * @param a_cols number of first matrix's columns
 * @param b a pointer-to-pointer to the second matrix
 * @param b_rows number of second matrix's rows
 * @param b_cols number of second matrix's columns
 * @param result_rows pointer to the result matrix rows
 * @param result_cols pointer to the result matrix columns
 *
 * `NOTE: it allocates memory`
 */
int** multiply_matrices(int** a, int a_rows, int a_cols, int** b, int b_rows, int b_cols, int* result_rows, int* result_cols)
{
    if (a_cols != b_rows)
    {
        fprintf(stderr, "Matrix multiplication not possible: incompatible dimensions\n");
        exit(EXIT_FAILURE);
    }

    *result_rows = a_rows;
    *result_cols = b_cols;

    // Allocate the result matrix, don't forget to use `free_matrix` later on
    int** result = allocate_matrix(*result_rows, *result_cols);

    for (int i = 0; i < *result_rows; i++)
    {
        for (int j = 0; j < *result_cols; j++)
        {
            result[i][j] = 0;
            for (int k = 0; k < a_cols; k++)
            {
                result[i][j] += a[i][k] * b[k][j];
            }
        }
    }
    return result;
}

/**
 * @brief Function to print a matrix
 *
 * @param matrix the pointer-to-pointer to the matrix to be printed out in stdout
 * @param rows
 * @param cols
 *
 * @return void
 */
void print_matrix(int** matrix, int rows, int cols)
{
    for (int i = 0; i < rows; i++)
    {
        for (int j = 0; j < cols; j++)
        {
            printf("%d ", matrix[i][j]);
        }
        printf("\n");
    }
}

int main()
{
    const char* filename = "input_mat.txt";
    FILE* file = fopen(filename, "r");
    if (!file)
    {
        perror("Failed to open file");
        exit(EXIT_FAILURE);
    }

    int a_rows, a_cols, b_rows, b_cols;
    int** matrix_a = load_matrix(file, &a_rows, &a_cols);
    int** matrix_b = load_matrix(file, &b_rows, &b_cols);

    // we no longer need the file
    fclose(file);

    int result_rows, result_cols;
    int** result_matrix = multiply_matrices(matrix_a, a_rows, a_cols, matrix_b, b_rows, b_cols, &result_rows, &result_cols);

    printf("Matrix A:\n");
    print_matrix(matrix_a, a_rows, a_cols);
    printf("Matrix B:\n");
    print_matrix(matrix_b, b_rows, b_cols);
    printf("Result of A x B:\n");
    print_matrix(result_matrix, result_rows, result_cols);

    // memory clean up
    free_matrix(matrix_a, a_rows);
    free_matrix(matrix_b, b_rows);
    free_matrix(result_matrix, result_rows);

    return 0;
}
