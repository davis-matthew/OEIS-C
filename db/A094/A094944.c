/*
 * A094944  A sequence with a(n)/a(n-1) converging to 7, generated from a
 *          semi-magic square.
 * https://oeis.org/A094944
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 11, u64 n <= 20, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation of OEIS A094944
   a(n) is the (0,0)-entry of M^n, where
   M = [ [1,4,2],
         [2,1,4],
         [4,2,1] ]
   (the first component of M^n * [1 0 0]^T) */
#include <stddef.h>

typedef struct { seq_t m[3][3]; } matrix3;

/* matrix multiplication: C = A * B */
static void mat_mul(const matrix3 *A, const matrix3 *B, matrix3 *C)
{
    int i, j, k;
    seq_t t;
    for (i = 0; i < 3; ++i) {
        for (j = 0; j < 3; ++j) {
            t = 0;
            for (k = 0; k < 3; ++k)
                t += A->m[i][k] * B->m[k][j];
            C->m[i][j] = t;
        }
    }
}

/* Matrix exponentiation by squaring: return M^n */
static void mat_pow(matrix3 *base, idx_t n)
{
    matrix3 result = { .m = { {1,0,0}, {0,1,0}, {0,0,1} } };
    matrix3 temp;
    while (n) {
        if (n & 1) {
            mat_mul(&result, base, &temp);
            result = temp;
        }
        mat_mul(base, base, &temp);
        *base = temp;
        n >>= 1;
    }
    *base = result;      /* now 'base' holds M^n */
}

seq_t a(idx_t n)
{
    /* n is 1‑based in OEIS; matrix exponent uses n as is */
    matrix3 M = { .m = { {1,4,2}, {2,1,4}, {4,2,1} } };
    mat_pow(&M, n);
    return M.m[0][0];   /* first entry of M^n */
}
