/*
 * A212404  Number of binary arrays of length 2*n+2 with no more than n ones
 *          in any length 2n subsequence (=50% duty cycle).
 * https://oeis.org/A212404
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2^(2*n+1) + binomial(2*n-2, n)
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 15, u64 n <= 31, u128 n <= 63,
 *            big any n
 * Helpers:   binom, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the number of binary arrays of length 2*n+2 with no more
   than n ones in any length 2n subsequence. */

#include "../utils/binom.h"
#include "../utils/pow2.h"

seq_t a(idx_t n) {
    /* 2^(2*n+1) */
    seq_t p = pow2(2 * n + 1);

    /* binomial(2*n-2, n) – returns 0 when k > n */
    seq_t b = binom(2 * n - 2, n);

    return p + b;
}
