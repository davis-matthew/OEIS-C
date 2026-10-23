/*
 * A181546  a(n) = Sum_{k=0..floor(n/2)} C(n-k,k)^4.
 * https://oeis.org/A181546
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..⌊n/2⌋} C(n-k,k)^4
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 14, u64 n <= 26, u128 n <= 49,
 *            big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  seq_t and idx_t are provided by the build.           */

#include "../utils/binom.h"

seq_t a(idx_t n)
{
    /* limit = floor(n/2) */
    idx_t limit = n >> 1;

    seq_t sum = (seq_t)0;

    for (idx_t k = 0; k <= limit; ++k) {
        /* Binomial coefficient C(n-k, k) */
        seq_t c = binom(n - k, k);

        /* c^4 : (c*c)^2 */
        seq_t t = c * c;       /* c^2   */
        seq_t term = t * t;    /* c^4   */

        sum += term;
    }

    return sum;
}
