/*
 * A058187  Expansion of (1+x)/(1-x^2)^4: duplicated tetrahedral numbers.
 * https://oeis.org/A058187
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = binomial(floor(n/2)+3, 3)
 * Range:     u8 n <= 13, u16 n <= 142, u32 n <= 880, u64 n <= 880,
 *            u128 n <= 880, big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binom.h"

seq_t a(idx_t n)
{
    /* floor(n/2) == n >> 1 for unsigned idx_t */
    idx_t k = n >> 1;
    return binom(k + 3, 3);
}
