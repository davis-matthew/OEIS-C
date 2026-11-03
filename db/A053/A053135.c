/*
 * A053135  Binomial coefficients C(2*n+6,6).
 * https://oeis.org/A053135
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = binomial(2*n+6, 6)
 * Range:     u8 n <= 2, u16 n <= 7, u32 n <= 58, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 47 terms through n = 961 (2026-11-01)
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    idx_t m = n * 2 + 6;
    return binomial(m, 6);
}
