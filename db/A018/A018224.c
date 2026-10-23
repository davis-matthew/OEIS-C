/*
 * A018224  a(n) = binomial(n, floor(n/2))^2 = A001405(n)^2.
 * https://oeis.org/A018224
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = binomial(n, floor(n/2))^2
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 18, u64 n <= 34, u128 n <= 67,
 *            big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binom.h"

seq_t a(idx_t n)
{
    idx_t k = n >> 1;               /* floor(n/2) */
    seq_t c = binom(n, k);          /* binomial coefficient */
    return c * c;                   /* square it */
}
