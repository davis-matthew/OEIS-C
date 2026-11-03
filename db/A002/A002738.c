/*
 * A002738  Coefficients for extrapolation.
 * https://oeis.org/A002738
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3*binomial(2*n+3,n)*binomial(n+3,n)
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 10, u64 n <= 25, u128 n <= 56,
 *            big any n
 * Validated: standard, pass, 38 terms through n = 948 (2026-11-01)
 * Helpers:   binomial, gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"
#include "../utils/gcd.h"

seq_t a(idx_t n) {
    seq_t b1 = binomial(2 * n + 3, n);
    seq_t b2 = binomial(n + 3, 3);
    return (seq_t)3 * b1 * b2;
}
