/*
 * A125407  Number of base 19 circular n-digit numbers with adjacent digits
 *          differing by 6 or less.
 * https://oeis.org/A125407
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = trace(A^n), where A is the 19×19 adjacency matrix * with
 *            A[i][j] = 1 iff |i-j| ≤ 6, and a(0) = 1. * a(1) = 19, a(2) =
 *            205, etc
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 17, u128 n <= 17,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

#define D 19

/* Matrix of seq_t in C/C++ */
typedef struct {
    seq_t v[D][D];
} matrix_t;

/* Set matrix to identity */
static void mat_identity(matrix_t *m)
{
    for (size_t i = 0; i < D; ++i)
        for (size_t j = 0; j < D; ++j)
            m->v[i][j] = (i == j) ? (seq_t)1 : (seq_t)0;
}

/* Multiply matrices a and b, store result in res */
static void mat_mul(const matrix_t *a, const matrix_t *b, matrix_t *res)
{
    for (size_t i = 0; i < D; ++i) {
        for (size_t j = 0; j < D; ++j) {
            seq_t sum = (seq_t)0;
            for (size_t k = 0; k < D; ++k)
                sum += a->v[i][k] * b->v[k][j];
            res->v[i][j] = sum;
        }
    }
}

/* Build the base adjacency matrix */
static void build_adj(matrix_t *adj)
{
    for (size_t i = 0; i < D; ++i)
        for (size_t j = 0; j < D; ++j)
            adj->v[i][j] = (i > j && (i - j) <= 6) || (i < j && (j - i) <= 6) || (i == j) ? (seq_t)1 : (seq_t)0;
}

/* Compute a(n) */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;

    /* Base adjacency matrix */
    static matrix_t base;
    static bool initialized = false;
    if (!initialized) {
        build_adj(&base);
        initialized = true;
    }

    /* Exponentiation by squaring */
    matrix_t result, temp;
    mat_identity(&result);
    matrix_t power = base;          /* copy of base */

    idx_t exp = n;
    while (exp != 0) {
        if (exp & (idx_t)1) {
            mat_mul(&result, &power, &temp);
            result = temp;
        }
        exp >>= 1;
        if (exp != 0) {             /* avoid extra square at end */
            mat_mul(&power, &power, &temp);
            power = temp;
        }
    }

    /* Trace of result matrix */
    seq_t trace = (seq_t)0;
    for (size_t i = 0; i < D; ++i)
        trace += result.v[i][i];

    return trace;
}
