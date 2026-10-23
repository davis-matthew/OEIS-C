/*
 * A125408  Number of base 20 circular n-digit numbers with adjacent digits
 *          differing by 6 or less.
 * https://oeis.org/A125408
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = trace(A^n) where A is the 20×20 adjacency matrix with
 *            A[i][j] = 1 if |i-j| ≤ 6 (digits differ by at most 6), = 0
 *            otherwise. a(0) = 1 (empty word)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 17, u128 n <= 17,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

typedef struct { seq_t m[400]; } matrix;

/* set matrix to identity (20×20) */
static void mat_identity(matrix *M)
{
    for (size_t k = 0; k < 400; ++k) M->m[k] = (seq_t)0;
    for (size_t i = 0; i < 20; ++i) M->m[i * 20 + i] = (seq_t)1;
}

/* copy matrix contents */
static void mat_copy(const matrix *src, matrix *dst)
{
    for (size_t k = 0; k < 400; ++k) dst->m[k] = src->m[k];
}

/* C = A * B  (20×20) */
static void mat_mult(const matrix *A, const matrix *B, matrix *C)
{
    for (size_t i = 0; i < 20; ++i) {
        for (size_t j = 0; j < 20; ++j) {
            seq_t sum = (seq_t)0;
            for (size_t k = 0; k < 20; ++k) {
                sum += A->m[i * 20 + k] * B->m[k * 20 + j];
            }
            C->m[i * 20 + j] = sum;
        }
    }
}

/* OEIS sequence function */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;          /* empty string */

    /* static base adjacency matrix, constructed once */
    static matrix base;
    static int built = 0;
    if (!built) {
        for (size_t i = 0; i < 20; ++i) {
            for (size_t j = 0; j < 20; ++j) {
                base.m[i * 20 + j] =
                    (((i > j) ? (i - j) : (j - i)) <= 6) ? (seq_t)1 : (seq_t)0;
            }
        }
        built = 1;
    }

    /* binary exponentiation: result = A^n */
    matrix result, cur, tmp;
    mat_identity(&result);
    mat_copy(&base, &cur);

    idx_t exp = n;
    while (exp) {
        if (exp & 1) {
            mat_mult(&result, &cur, &tmp);
            mat_copy(&tmp, &result);
        }
        exp >>= 1;
        if (!exp) break;
        mat_mult(&cur, &cur, &tmp);
        mat_copy(&tmp, &cur);
    }

    /* trace of A^n */
    seq_t tr = (seq_t)0;
    for (size_t i = 0; i < 20; ++i) tr += result.m[i * 20 + i];
    return tr;
}
