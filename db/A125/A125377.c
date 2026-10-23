/*
 * A125377  Number of base 14 circular n-digit numbers with adjacent digits
 *          differing by 5 or less.
 * https://oeis.org/A125377
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = number of closed walks of length n on the graph with
 *            vertices 0..13 and edges between vertices whose difference (in
 *            absolute value) is ≤ 5. a(0)=1
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 18, u128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>

/* 14×14 matrix of seq_t.  */
typedef struct {
    seq_t m[14][14];
} Matrix;

/* Multiply matrices: res = a * b. */
static void mat_mul(const Matrix *a, const Matrix *b, Matrix *res)
{
    seq_t tmp[14][14];
    for (idx_t i = 0; i < 14; ++i) {
        for (idx_t j = 0; j < 14; ++j) {
            seq_t sum = (seq_t)0;
            for (idx_t k = 0; k < 14; ++k) {
                sum += a->m[i][k] * b->m[k][j];
            }
            tmp[i][j] = sum;
        }
    }
    for (idx_t i = 0; i < 14; ++i) {
        for (idx_t j = 0; j < 14; ++j) {
            res->m[i][j] = tmp[i][j];
        }
    }
}

/* Return a(n). */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;

    static bool initialized = false;
    static Matrix adj;

    if (!initialized) {
        initialized = true;
        for (idx_t i = 0; i < 14; ++i) {
            for (idx_t j = 0; j < 14; ++j) {
                idx_t diff = (i > j) ? (i - j) : (j - i);
                adj.m[i][j] = (diff <= 5) ? (seq_t)1 : (seq_t)0;
            }
        }
    }

    /* Matrix exponentiation: compute adj^n. */
    Matrix result, base;
    /* Initialise result as identity and base as the adjacency matrix. */
    for (idx_t i = 0; i < 14; ++i) {
        for (idx_t j = 0; j < 14; ++j) {
            result.m[i][j] = (i == j) ? (seq_t)1 : (seq_t)0;
            base.m[i][j]   = adj.m[i][j];
        }
    }

    idx_t exp = n;
    while (exp) {
        if (exp & (idx_t)1) {
            mat_mul(&result, &base, &result);
        }
        exp >>= 1;
        if (exp) {
            mat_mul(&base, &base, &base);
        }
    }

    /* The desired number is the trace of adj^n. */
    seq_t total = (seq_t)0;
    for (idx_t i = 0; i < 14; ++i) {
        total += result.m[i][i];
    }
    return total;
}
