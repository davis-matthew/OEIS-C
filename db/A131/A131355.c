/*
 * A131355  Partial sums of A065423 plus one.
 * https://oeis.org/A131355
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 1 + binomial(n,2) - binomial(floor(n/2)+1,2)
 * Range:     u8 n <= 16, u16 n <= 55, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binom.h"

seq_t a(idx_t n)
{
    seq_t bin_n2 = binom(n, 2);
    idx_t half = n >> 1;                /* floor(n/2) */
    seq_t bin_half_plus1_2 = binom(half + 1, 2);
    return (seq_t)1 + bin_n2 - bin_half_plus1_2;
}
