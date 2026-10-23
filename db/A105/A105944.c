/*
 * A105944  a(n) = binomial(n+8,n)*binomial(n+11,8).
 * https://oeis.org/A105944
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = binomial(n+8,8) * binomial(n+11,8)
 * Range:     u8 none, u16 n <= 2, u32 n <= 9, u64 n <= 54, u128 n <= 957,
 *            big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

// This implementation uses the generic binomial definition from utils/binom.h
#include "../utils/binom.h"

seq_t a(idx_t n)
{
    // Compute the two binomial coefficients.
    seq_t c1 = binom(n + 8, 8);
    seq_t c2 = binom(n + 11, 8);

    // Return the product.
    return c1 * c2;
}
