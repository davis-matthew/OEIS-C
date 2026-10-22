/*
 * A002350  Take solution to Pellian equation x^2 - n*y^2 = 1 with smallest
 *          positive y and x >= 0; sequence gives a(n) = x, or 1 if n is a
 *          square. A002349 gives values of y.
 * https://oeis.org/A002350
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, nice, easy
 * Formula:   continued fraction of sqrt(n) = [a0; a1, ..., a_r] where a_r =
 *            2*a0 ends the period; with convergents p(-1)=1, p(0)=a0,
 *            p(k)=a_k*p(k-1)+p(k-2): a(n) = p(r-1) if the period length r is
 *            even, p(2r-1) if r is odd; a(n) = 1 if n is a square
 * Range:     u8 n <= 12, u16 n <= 52, u32 n <= 108, u64 n <= 276,
 *            u128 n <= 1020, big any n
 * Helpers:   a2350_isqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/a2350_isqrt.h"

seq_t a(idx_t n) {
    uint64_t N = (uint64_t)n;
    uint64_t a0 = a2350_isqrt(N);
    if (a0 * a0 == N) return (seq_t)1;

    uint64_t m = 0, d = 1, ak = a0;
    seq_t p_prev = (seq_t)1, p = (seq_t)a0;
    uint64_t k = 0;
    for (;;) {
        k++;
        m = d * ak - m;
        d = (N - m * m) / d;
        ak = (a0 + m) / d;
        if (ak == 2 * a0 && (k % 2) == 0) return p;
        seq_t p_next = (seq_t)ak * p + p_prev;
        p_prev = p;
        p = p_next;
    }
}
