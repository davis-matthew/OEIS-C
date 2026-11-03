/*
 * A002483  Expansion of Jacobi theta function {theta_1}'(q) in powers of
 *          q^(1/4).
 * https://oeis.org/A002483
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, changed
 * Formula:   a(n) = 2*(2k+1)*(-1)^k if n = (2k+1)^2, else 0 (G.f.:
 *            2*x*Product_{k>0} (1 - x^(8*k))^3)
 * Range:     i8 n <= 4224, i16 n <= 10201, i32 n <= 10201, i64 n <= 10201,
 *            i128 n <= 10201, big any n
 * Validated: standard, pass, 101 terms through n = 9305 (2026-11-01)
 * Helpers:   isqrt_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/isqrt_u64.h"

seq_t a(idx_t n) {
    uint64_t s, k;
    seq_t v;
    if (n == 0) return 0;
    s = isqrt_u64((uint64_t)n);
    if (s * s != (uint64_t)n || (s & 1) == 0) return 0;
    k = (s - 1) / 2;
    v = (seq_t)2 * (seq_t)s;
    if (k & 1) v = -v;
    return v;
}
