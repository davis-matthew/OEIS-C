/*
 * A002738  Coefficients for extrapolation.
 * https://oeis.org/A002738
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3*binomial(2*n+3,n)*binomial(n+3,n)
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 10, u64 n <= 25, u128 n <= 56,
 *            big any n
 * Helpers:   a002738_binom, a002738_gcd_s
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/a002738_binom.h"
#include "../utils/a002738_gcd_s.h"

seq_t a(idx_t n) {
    seq_t b1 = a002738_binom(2 * n + 3, n);
    seq_t b2 = a002738_binom(n + 3, 3);
    return (seq_t)3 * b1 * b2;
}
