/*
 * A125348  Number of base 11 circular n-digit numbers with adjacent digits
 *          differing by 4 or less.
 * https://oeis.org/A125348
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = trace(A^n) where A[i][j] = 1 if |i-j| <= 4, else 0. For n
 *            = 0, a(0) = 1 (empty word). For n >= 1, a(n) counts closed walks
 *            of length n on the graph with vertices 0..10 and edges between
 *            vertices of distance at most 4 in the number line
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 10, u64 n <= 19, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

/* multiply two 11×11 matrices: res = a * b */
static void mat_mul(const seq_t a[11][11], const seq_t b[11][11],
                    seq_t res[11][11]) {
    seq_t sum;
    for (idx_t i = 0; i < 11; ++i)
        for (idx_t j = 0; j < 11; ++j)
            res[i][j] = 0;
    for (idx_t i = 0; i < 11; ++i)
        for (idx_t k = 0; k < 11; ++k) {
            seq_t aik = a[i][k];
            if (aik == (seq_t)0) continue;
            for (idx_t j = 0; j < 11; ++j) {
                res[i][j] += aik * b[k][j];
            }
        }
}

/* copy matrix src to dst */
static void mat_copy(seq_t dst[11][11], const seq_t src[11][11]) {
    for (idx_t i = 0; i < 11; ++i)
        for (idx_t j = 0; j < 11; ++j)
            dst[i][j] = src[i][j];
}

seq_t a(idx_t n) {
    if (n == 0) return 1;

    /* identity matrix */
    seq_t res[11][11];
    for (idx_t i = 0; i < 11; ++i)
        for (idx_t j = 0; j < 11; ++j)
            res[i][j] = (i == j) ? 1 : 0;

    /* adjacency matrix */
    seq_t cur[11][11];
    for (idx_t i = 0; i < 11; ++i)
        for (idx_t j = 0; j < 11; ++j)
            cur[i][j] = ( (i > j ? i - j : j - i) <= 4 ) ? 1 : 0;

    seq_t temp[11][11];
    idx_t exp = n;
    while (exp > 0) {
        if (exp & 1) {
            mat_mul(res, cur, temp);
            mat_copy(res, temp);
        }
        exp >>= 1;
        if (exp) {
            mat_mul(cur, cur, temp);
            mat_copy(cur, temp);
        }
    }

    /* trace of res */
    seq_t trace = 0;
    for (idx_t i = 0; i < 11; ++i)
        trace += res[i][i];
    return trace;
}
