/*
 * A126456  Column 1 of triangle A126454; a(n) = C( C(n+3,3) + 1, n).
 * https://oeis.org/A126456
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = binom( binom(n+3,3) + 1, n)
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 6, u64 n <= 10, u128 n <= 13,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    /* Compute N = C(n+3,3) + 1 = ((n+3)(n+2)(n+1))/6 + 1 */
    idx_t N = ((n + 3) * (n + 2) * (n + 1) / 6) + 1;
    /* a(n) is C(N, n) */
    return binomial(N, n);
}
