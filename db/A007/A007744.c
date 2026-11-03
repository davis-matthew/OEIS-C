/*
 * A007744  Expansion of (1+6*x)/(1-4*x)^(7/2).
 * https://oeis.org/A007744
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = binomial(2n+3, n) * binomial(n+3, 3)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 11, u64 n <= 26, u128 n <= 56,
 *            big any n
 * Validated: standard, pass, 39 terms through n = 96 (2026-11-01)
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/binomial.h"

seq_t a(idx_t n) {
    return binomial(2*n+3, n) * binomial(n+3, 3);
}
