/*
 * A002655  Expansion of Product_{i >= 1} (1 - q^i)*(1 - q^(7*i)).
 * https://oeis.org/A002655
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   G.f. Product_{k>0} (1 - x^k)*(1 - x^(7*k)); by Euler's
 *            pentagonal theorem a(n) = Sum_{j} (-1)^j * e(n - 7*j*(3*j-1)/2),
 *            j over all integers, where e(m) = (-1)^k if m = k*(3*k-1)/2 for
 *            some integer k, else 0
 * Range:     i8 n <= 10000, i16 n <= 10000, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Validated: standard, pass, 98 terms through n = 9715 (2026-11-01)
 * Helpers:   isqrt_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/isqrt_u64.h"

/* coefficient of x^m in Product_{k>0} (1 - x^k) */
static int a002655_euler(uint64_t m) {
    uint64_t t = 24 * m + 1;
    uint64_t s = isqrt_u64(t);
    uint64_t k;
    if (s * s != t) return 0;
    if (s % 6 == 1) k = (s - 1) / 6;
    else if (s % 6 == 5) k = (s + 1) / 6;
    else return 0;
    return (k & 1) ? -1 : 1;
}

seq_t a(idx_t n) {
    long long sum = 0;
    uint64_t k;
    /* k = 0 term: p = 0 */
    sum += a002655_euler(n);
    for (k = 1;; k++) {
        uint64_t p1 = k * (3 * k - 1) / 2;
        uint64_t p2 = k * (3 * k + 1) / 2;
        int sg = (k & 1) ? -1 : 1;
        if (7 * p1 > n) break;
        sum += sg * a002655_euler(n - 7 * p1);
        if (7 * p2 <= n) sum += sg * a002655_euler(n - 7 * p2);
    }
    return (seq_t)sum;
}
