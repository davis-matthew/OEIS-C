/*
 * A002349  Take solution to Pellian equation x^2 - n*y^2 = 1 with smallest
 *          positive y and x >= 0; sequence gives a(n) = y, or 0 if n is a
 *          square. A002350 gives values of x.
 * https://oeis.org/A002349
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, nice, easy
 * Formula:   continued fraction expansion of sqrt(n): m(k+1) = d(k)*a(k) -
 *            m(k), d(k+1) = (n - m(k+1)^2)/d(k), a(k+1) = floor((a0 +
 *            m(k+1))/d(k+1)), with q(-1) = 0, q(0) = 1, q(k) = a(k)*q(k-1) +
 *            q(k-2); a(n) = q(k) for the first odd k with d(k+1) = 1 (then
 *            p(k)^2 - n*q(k)^2 = +1); a(n) = 0 if n is a square
 * Range:     u8 n <= 28, u16 n <= 60, u32 n <= 108, u64 n <= 396,
 *            u128 n <= 1020, big any n
 * Helpers:   a002349_isqrt_u
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/a002349_isqrt_u.h"

seq_t a(idx_t n) {
    uint64_t N = (uint64_t)n;
    uint64_t a0 = a002349_isqrt_u(N);
    uint64_t m = 0, d = 1, av = a0;
    uint64_t k = 0;
    seq_t q_prev = 0, q_curr = 1;

    if (a0 * a0 == N) return 0;

    for (;;) {
        m = d * av - m;
        d = (N - m * m) / d;
        av = (a0 + m) / d;
        if (d == 1 && (k & 1)) return q_curr;
        {
            seq_t q_next = q_curr * (seq_t)av + q_prev;
            q_prev = q_curr;
            q_curr = q_next;
        }
        k++;
    }
}
