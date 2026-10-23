/*
 * A189770  Number of one-sided prudent walks from (0,0) to (n,n), with 4+n
 *          east steps, 4 west steps and n north steps.
 * https://oeis.org/A189770
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, walk
 * Formula:   a(n) = n*(n+1)*(n+2)*(n+3)/24 * binom(2*n, n)
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 12, u64 n <= 26, u128 n <= 56,
 *            big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binom.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;  /* sequence is defined from n = 1, but 0 is harmless */

    /* Compute the binomial coefficient C(2n, n) */
    seq_t bin = binom(2 * n, n);

    /* Compute the product n*(n+1)*(n+2)*(n+3)/24 */
    seq_t factor = (seq_t)n;
    factor *= (n + 1);
    factor *= (n + 2);
    factor *= (n + 3);
    factor /= 24;

    /* Multiply to obtain the term */
    bin *= factor;
    return bin;
}
