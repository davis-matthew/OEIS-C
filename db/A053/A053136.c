/*
 * A053136  Binomial coefficients C(2*n+7,7).
 * https://oeis.org/A053136
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = binomial(2*n+7,7)
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 28, u64 n <= 200, u128 n <= 200,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    idx_t N = n + n + 7;
    return binomial(N, 7);
}
