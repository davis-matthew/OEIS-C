/*
 * A104478  a(n) = binomial(n+8,8)*binomial(n+12,8).
 * https://oeis.org/A104478
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = binomial(n+8,8) * binomial(n+12,8)
 * Range:     u8 none, u16 n <= 1, u32 n <= 8, u64 n <= 21, u128 n <= 21,
 *            big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binom.h"

seq_t a(idx_t n)
{
    /* compute the two binomial coefficients and multiply */
    seq_t c1 = binom(n + 8, 8);
    seq_t c2 = binom(n + 12, 8);
    return c1 * c2;
}
