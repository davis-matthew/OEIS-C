/*
 * A000727  Expansion of Product_{k >= 1} (1 - x^k)^4.
 * https://oeis.org/A000727
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = Sum_{m>=0, m(m+1)/2<=n} (-1)^m*(2m+1)*e(n - m(m+1)/2),
 *            where e(r) = (-1)^k if r = k(3k-1)/2 for an integer k, else 0
 *            (Jacobi's identity for prod (1-x^k)^3 times Euler's pentagonal
 *            theorem for prod (1-x^k))
 * Range:     i8 n <= 325, i16 n <= 10000, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Validated: standard, pass, 98 terms through n = 9451 (2026-11-01)
 * Helpers:   isqrt_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/isqrt_u64.h"

/* coefficient of x^r in prod (1-x^k): 0, 1 or -1 */
static int pent_coef(uint64_t r) {
    uint64_t d = 24 * r + 1;
    uint64_t s = isqrt_u64(d);
    uint64_t k;
    if (s * s != d) return 0;
    if (s % 6 == 5) k = (s + 1) / 6;
    else if (s % 6 == 1) k = (s - 1) / 6;
    else return 0;
    return (k & 1) ? -1 : 1;
}

seq_t a(idx_t n) {
    seq_t r = 0;
    idx_t m, T;
    for (m = 0; (T = m * (m + 1) / 2) <= n; m++) {
        int e = pent_coef(n - T);
        if (e == 0) continue;
        seq_t term = (seq_t)(2 * m + 1);
        if ((m & 1) ? (e > 0) : (e < 0)) r -= term;
        else r += term;
    }
    return r;
}
