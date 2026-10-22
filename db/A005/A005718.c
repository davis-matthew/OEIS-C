/*
 * A005718  Quadrinomial coefficients: C(2+n,n) + C(3+n,n) + C(4+n,n).
 * https://oeis.org/A005718
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = C(2+n,n) + C(3+n,n) + C(4+n,n)
 * Range:     u8 n <= 5, u16 n <= 31, u32 n <= 505, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binom.h"

seq_t a(idx_t n) {
    return binom(n + 2, n) + binom(n + 3, n) + binom(n + 4, n);
}
