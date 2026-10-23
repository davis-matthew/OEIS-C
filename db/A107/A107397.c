/*
 * A107397  a(n) = binomial(n+6, 6) * binomial(n+8, 6).
 * https://oeis.org/A107397
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = binomial(n+6,6)*binomial(n+8,6)
 * Range:     u8 n <= 0, u16 n <= 3, u32 n <= 14, u64 n <= 116, u128 n <= 1000,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    seq_t c1 = binomial(n + 6, 6);
    seq_t c2 = binomial(n + 8, 6);
    return c1 * c2;
}
