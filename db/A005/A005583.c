/*
 * A005583  Coefficients of Chebyshev polynomials.
 * https://oeis.org/A005583
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = binomial(n+3,5) + 2*binomial(n+3,4)
 * Range:     u8 n <= 5, u16 n <= 21, u32 n <= 164, u64 n <= 172,
 *            u128 n <= 172, big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binom.h"

seq_t a(idx_t n) {
    return binom(n + 3, 5) + 2 * binom(n + 3, 4);
}
