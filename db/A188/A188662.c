/*
 * A188662  Binomial coefficients: a(n) = binomial(3*n,n)^2.
 * https://oeis.org/A188662
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, changed
 * Formula:   a(n) = binomial(3*n, n)^2
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 6, u64 n <= 12, u128 n <= 24,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    seq_t c = binomial((idx_t)(3 * n), (idx_t)n);
    return c * c;
}
