/*
 * A065129  a(n) is the least m such that m/A003285(m) = n, or 0 if no such m
 *          exists.
 * https://oeis.org/A065129
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = least m = n*P with A003285(m) = P (P = 1, 2, ...), else
 *            0; search over P <= 100*n, since period(m) stays far below
 *            10*sqrt(m) for m in this range
 * Range:     u8 n <= 22, u16 n <= 360, u32 n <= 406, u64 n <= 406,
 *            u128 n <= 406, big any n
 * Validated: standard, pass, 82 terms through n = 382 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>

static uint64_t A065129_isqrt_u64(uint64_t n) {
    if (n < 2) return n;
    uint64_t x = n, y;
    while (1) {
        y = (x + n / x) / 2;
        if (y >= x) break;
        x = y;
    }
    return x;
}

/* true iff the continued-fraction period of sqrt(N) equals P (N nonsquare, a0 = floor(sqrt N));
   stops as soon as the count passes P */
static bool A065129_period_is(uint64_t N, uint64_t a0, uint64_t P) {
    uint64_t m = 0, d = 1, av = a0, cnt = 0;
    uint64_t twice = 2 * a0;
    while (1) {
        m = d * av - m;
        d = (N - m * m) / d;
        av = (a0 + m) / d;
        cnt++;
        if (av == twice) break;
        if (cnt >= P) return false;
    }
    return cnt == P;
}

seq_t a(idx_t n) {
    uint64_t N = (uint64_t)n;
    uint64_t limit = 100 * N + 100;
    for (uint64_t P = 1; P <= limit; P++) {
        uint64_t m = N * P;
        uint64_t s = A065129_isqrt_u64(m);
        if (s * s == m) continue;
        if (A065129_period_is(m, s, P)) return (seq_t)m;
    }
    return (seq_t)0;
}
