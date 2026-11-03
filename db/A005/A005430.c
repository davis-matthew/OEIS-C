/*
 * A005430  Apéry numbers: n*C(2*n,n).
 * https://oeis.org/A005430
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   a(n) = n*C(2*n,n)
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 15, u64 n <= 31, u128 n <= 62,
 *            big any n
 * Validated: standard, pass, 41 terms through n = 195 (2026-11-01)
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/binomial.h"

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)0;
    return (seq_t)n * binomial(2 * n, n);
}
