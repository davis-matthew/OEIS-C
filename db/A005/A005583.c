/*
 * A005583  Coefficients of Chebyshev polynomials.
 * https://oeis.org/A005583
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = binomial(n+3,5) + 2*binomial(n+3,4)
 * Range:     u8 n <= 5, u16 n <= 21, u32 n <= 172, u64 n <= 172,
 *            u128 n <= 172, big any n
 * Validated: standard, pass, 56 terms through n = 165 (2026-11-01)
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/binomial.h"

seq_t a(idx_t n) {
    return binomial(n + 3, 5) + 2 * binomial(n + 3, 4);
}
