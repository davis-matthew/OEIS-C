/*
 * A124828  Number of base 7 circular n-digit numbers with adjacent digits
 *          differing by 2 or less.
 * https://oeis.org/A124828
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 15, u64 n <= 30, u128 n <= 60,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A124828: Number of base‑7 circular n‑digit numbers with adjacent digits
    differing by 2 or less.
    Computation: a(n) = trace( M^n ) where M is the 7×7 adjacency matrix
    with M[i][j] = 1 iff |i-j| ≤ 2 (otherwise 0).  a(0) = 1 is defined
    separately.  M^n is computed by binary exponentiation.  The function
    works for any integer build of seq_t (8–128 bits or arbitrary precision). */

#include <stdint.h>

static void matrix_mult(const seq_t a[7][7], const seq_t b[7][7], seq_t res[7][7])
{
    for (int i = 0; i < 7; ++i) {
        for (int j = 0; j < 7; ++j) {
            seq_t sum = 0;
            for (int k = 0; k < 7; ++k)
                sum += a[i][k] * b[k][j];
            res[i][j] = sum;
        }
    }
}

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;

    /* Build adjacency matrix M (bool → 1 or 0). */
    seq_t base[7][7];
    for (int i = 0; i < 7; ++i) {
        for (int j = 0; j < 7; ++j) {
            int diff = i > j ? i - j : j - i;
            base[i][j] = (diff <= 2) ? (seq_t)1 : (seq_t)0;
        }
    }

    /* Identity matrix */
    seq_t result[7][7];
    for (int i = 0; i < 7; ++i) {
        for (int j = 0; j < 7; ++j)
            result[i][j] = (i == j) ? (seq_t)1 : (seq_t)0;
    }

    idx_t exp = n;
    seq_t temp[7][7];
    seq_t ipw[7][7];    /* for base squared */

    while (exp > 0) {
        if (exp & 1) {
            matrix_mult(result, base, temp);
            for (int i = 0; i < 7; ++i)
                for (int j = 0; j < 7; ++j)
                    result[i][j] = temp[i][j];
        }
        matrix_mult(base, base, ipw);
        for (int i = 0; i < 7; ++i)
            for (int j = 0; j < 7; ++j)
                base[i][j] = ipw[i][j];
        exp >>= 1;
    }

    /* trace of M^n */
    seq_t trace = 0;
    for (int i = 0; i < 7; ++i)
        trace += result[i][i];
    return trace;
}
