/*
 * A053135  Binomial coefficients C(2*n+6,6).
 * https://oeis.org/A053135
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = binomial(2*n+6, 6)
 * Range:     u8 n <= 1, u16 n <= 5, u32 n <= 57, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    idx_t m = n * 2 + 6;
    return binomial(m, 6);
}
