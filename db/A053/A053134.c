/*
 * A053134  Binomial coefficients C(2*n+4,4).
 * https://oeis.org/A053134
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = binomial(2*n+4,4)
 * Range:     u8 n <= 3, u16 n <= 16, u32 n <= 200, u64 n <= 200,
 *            u128 n <= 200, big any n
 * Validated: standard, pass, 53 terms through n = 197 (2026-11-01)
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    /* Compute the binomial coefficient C(2*n+4, 4) using the supplied utility. */
    return binomial(2*n + 4, 4);
}
