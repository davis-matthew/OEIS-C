/*
 * A125440  Number of base 28 circular n-digit numbers with adjacent digits
 *          differing by 7 or less.
 * https://oeis.org/A125440
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = trace(M^n) for n >= 1, a(0) = 1, where M[i][j] = 1 if
 *            |i-j| <= 7 (i,j in 0..27), else 0
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 8, u64 n <= 16, u128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;

    /* Dimension of the adjacency matrix (base 28) */
    const int D = 28;
    /* Current base matrix (adjacency) and result matrix (powers) */
    seq_t base[D][D];
    seq_t res[D][D];
    seq_t tmp[D][D];
    /* Initialize adjacency matrix */
    for (idx_t i = 0; i < (idx_t)D; ++i) {
        for (idx_t j = 0; j < (idx_t)D; ++j) {
            if (i >= j)
                base[i][j] = ((i - j) <= 7 ? 1 : 0);
            else
                base[i][j] = ((j - i) <= 7 ? 1 : 0);
        }
    }
    /* Initialize result to identity matrix */
    for (idx_t i = 0; i < (idx_t)D; ++i)
        for (idx_t j = 0; j < (idx_t)D; ++j)
            res[i][j] = 0;
    for (idx_t i = 0; i < (idx_t)D; ++i)
        res[i][i] = 1;

    idx_t exp = n;
    /* Fast exponentiation of matrices, keeping only the trace */
    while (exp) {
        if (exp & 1) {
            /* res = res * base */
            for (idx_t i = 0; i < (idx_t)D; ++i) {
                for (idx_t j = 0; j < (idx_t)D; ++j) {
                    seq_t sum = 0;
                    for (idx_t k = 0; k < (idx_t)D; ++k)
                        sum += res[i][k] * base[k][j];
                    tmp[i][j] = sum;
                }
            }
            for (idx_t i = 0; i < (idx_t)D; ++i)
                for (idx_t j = 0; j < (idx_t)D; ++j)
                    res[i][j] = tmp[i][j];
        }
        /* base = base * base */
        for (idx_t i = 0; i < (idx_t)D; ++i) {
            for (idx_t j = 0; j < (idx_t)D; ++j) {
                seq_t sum = 0;
                for (idx_t k = 0; k < (idx_t)D; ++k)
                    sum += base[i][k] * base[k][j];
                tmp[i][j] = sum;
            }
        }
        for (idx_t i = 0; i < (idx_t)D; ++i)
            for (idx_t j = 0; j < (idx_t)D; ++j)
                base[i][j] = tmp[i][j];
        exp >>= 1;
    }

    /* Sum diagonal entries: trace of M^n */
    seq_t trace = 0;
    for (idx_t i = 0; i < (idx_t)D; ++i)
        trace += res[i][i];
    return trace;
}
