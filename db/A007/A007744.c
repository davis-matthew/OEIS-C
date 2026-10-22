/*
 * A007744  Expansion of (1+6*x)/(1-4*x)^(7/2).
 * https://oeis.org/A007744
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = binomial(2n+3, n) * binomial(n+3, 3)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 11, u64 n <= 26, u128 n <= 56,
 *            big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binom.h"

seq_t a(idx_t n) {
    return binom(2*n+3, n) * binom(n+3, 3);
}
