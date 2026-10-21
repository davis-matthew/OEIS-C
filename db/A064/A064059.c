/*
 * A064059  Seventh column of Catalan triangle A009766.
 * https://oeis.org/A064059
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = binomial(n+13,6) - 2*binomial(n+12,5)
 * Range:     u8 none, u16 n <= 3, u32 n <= 112, u64 n <= 1000, u128 n <= 1000,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    seq_t c5 = binomial(n + 12, 5);  /* (n+12 choose 5) */
    seq_t c6 = binomial(n + 13, 6);  /* (n+13 choose 6) */
    return c6 - (c5 << 1);           /* subtract 2*c5 */
}
