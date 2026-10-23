/*
 * A125391  Number of base 28 circular n-digit numbers with adjacent digits
 *          differing by 5 or less.
 * https://oeis.org/A125391
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = trace(A^n) where A is the 28×28 adjacency matrix with
 *            entries A[i][j] = 1 if |i-j| ≤ 5, otherwise 0. a(0) is defined
 *            as 1
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 9, u64 n <= 17, u128 n <= 17,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>

typedef struct {
    seq_t a[28][28];
} Matrix;

/* Construct the identity matrix */
static Matrix identity(void) {
    Matrix I = {0};
    seq_t one = (seq_t)1;
    for (idx_t i = 0; i < 28; ++i) I.a[i][i] = one;
    return I;
}

/* Initialize the adjacency matrix A */
static void init_base(Matrix *M) {
    seq_t one = (seq_t)1;
    seq_t zero = (seq_t)0;
    for (idx_t i = 0; i < 28; ++i) {
        for (idx_t j = 0; j < 28; ++j) {
            idx_t d = (i > j) ? (i - j) : (j - i);
            M->a[i][j] = (d <= 5) ? one : zero;
        }
    }
}

/* Multiply two 28×28 matrices: C = A * B */
static void mat_mult(const Matrix *A, const Matrix *B, Matrix *C) {
    seq_t temp[28][28] = {0};
    for (idx_t i = 0; i < 28; ++i) {
        for (idx_t k = 0; k < 28; ++k) {
            if (A->a[i][k] == 0) continue;
            for (idx_t j = 0; j < 28; ++j) {
                temp[i][j] += A->a[i][k] * B->a[k][j];
            }
        }
    }
    /* copy temporary result to output */
    for (idx_t i = 0; i < 28; ++i)
        for (idx_t j = 0; j < 28; ++j)
            C->a[i][j] = temp[i][j];
}

/* Fast exponentiation: compute A^exp */
static void mat_pow(const Matrix *A, idx_t exp, Matrix *res) {
    Matrix result = identity();
    Matrix base;
    for (idx_t i = 0; i < 28; ++i) {
        for (idx_t j = 0; j < 28; ++j)
            base.a[i][j] = A->a[i][j];
    }
    while (exp) {
        if (exp & 1ULL) {
            Matrix tmp;
            mat_mult(&result, &base, &tmp);
            result = tmp;
        }
        exp >>= 1ULL;
        if (exp) {
            Matrix tmp;
            mat_mult(&base, &base, &tmp);
            base = tmp;
        }
    }
    *res = result;
}

/* Compute the trace of a 28×28 matrix */
static seq_t mat_trace(const Matrix *M) {
    seq_t tr = (seq_t)0;
    for (idx_t i = 0; i < 28; ++i) tr += M->a[i][i];
    return tr;
}

/* The required sequence function */
seq_t a(idx_t n) {
    if (n == 0) return (seq_t)1;

    static bool init = false;
    static Matrix base;
    if (!init) {
        init_base(&base);
        init = true;
    }

    Matrix power;
    mat_pow(&base, n, &power);
    return mat_trace(&power);
}
