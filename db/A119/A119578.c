/*
 * A119578  a(n) = (n + n^2)*binomial(2*n,n)/2.
 * https://oeis.org/A119578
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(n+1)/2 * binomial(2*n,n)
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 14, u64 n <= 29, u128 n <= 60,
 *            big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binom.h"

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;

    /* triangular number T = n*(n+1)/2 */
    seq_t tri = ((seq_t)n * ((seq_t)n + (seq_t)1)) / (seq_t)2;

    /* central binomial coefficient C(2n,n) */
    seq_t bin = binom((idx_t)(2 * n), n);

    /* final result */
    return tri * bin;
}
