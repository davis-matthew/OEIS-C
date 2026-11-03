/*
 * A001451  a(n) = (5*n)!/((3*n)!*n!*n!).
 * https://oeis.org/A001451
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, walk
 * Formula:   a(n) = binomial(4*n,n)*binomial(5*n,n)
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 5, u64 n <= 10, u128 n <= 19,
 *            big any n
 * Validated: standard, pass, 31 terms through n = 200 (2026-11-01)
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"

seq_t a(idx_t n) {
    seq_t b1 = binomial(4 * n, n);
    seq_t b2 = binomial(5 * n, n);
    return b1 * b2;
}
