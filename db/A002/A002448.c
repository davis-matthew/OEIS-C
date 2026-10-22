/*
 * A002448  Expansion of Jacobi theta function theta_4(x).
 * https://oeis.org/A002448
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy, changed
 * Formula:   G.f.: Sum_{k in Z} (-1)^k * x^(k^2); a(0) = 1, a(k^2) = 2*(-1)^k
 *            for k > 0, else a(n) = 0
 * Range:     i8 n <= 10000, i16 n <= 10000, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   a002448_isqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/a002448_isqrt.h"

seq_t a(idx_t n) {
    uint64_t k;
    if (n == 0) return (seq_t)1;
    k = a002448_isqrt((uint64_t)n);
    if (k * k != (uint64_t)n) return (seq_t)0;
    return (k & 1) ? (seq_t)(-2) : (seq_t)2;
}
