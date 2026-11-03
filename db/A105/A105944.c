/*
 * A105944  a(n) = binomial(n+8,n)*binomial(n+11,8).
 * https://oeis.org/A105944
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = binomial(n+8,8) * binomial(n+11,8)
 * Range:     u8 n <= 0, u16 n <= 2, u32 n <= 9, u64 n <= 54, u128 n <= 957,
 *            big any n
 * Validated: standard, pass, 38 terms through n = 982 (2026-11-01)
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

// This implementation uses the generic binomial definition from utils/binom.h
#include <sys/types.h>
#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    // Compute the two binomial coefficients.
    seq_t c1 = binomial(n + 8, 8);
    seq_t c2 = binomial(n + 11, 8);

    // Return the product.
    return c1 * c2;
}
