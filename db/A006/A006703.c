/*
 * A006703  Solution to Pellian: y such that x^2 - n*y^2 = +-1.
 * https://oeis.org/A006703
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 0 if n is a square; otherwise a(n) = q(r-1), the
 *            denominator of the (r-1)-th convergent of the continued fraction
 *            of sqrt(n), where r is its period length (q(-1) = 0, q(0) = 1,
 *            q(k) = c(k)*q(k-1) + q(k-2))
 * Range:     u8 n <= 30, u16 n <= 93, u32 n <= 210, u64 n <= 630,
 *            u128 n <= 2010, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static uint64_t isqrt_u64(uint64_t x) {
    if (x < 2) return x;
    uint64_t y = x;
    for (;;) {
        uint64_t y1 = (y + x / y) / 2;
        if (y1 >= y) return y;
        y = y1;
    }
}

seq_t a(idx_t n) {
    uint64_t nn = (uint64_t)n;
    uint64_t a0 = isqrt_u64(nn);
    while (a0 * a0 > nn) a0--;
    while ((a0 + 1) * (a0 + 1) <= nn) a0++;
    if (a0 * a0 == nn) return (seq_t)0;

    uint64_t m = 0, d = 1, c = a0;
    seq_t q_prev = (seq_t)0;
    seq_t q = (seq_t)1;

    for (;;) {
        m = d * c - m;
        d = (nn - m * m) / d;
        c = (a0 + m) / d;
        if (c == 2 * a0) return q;
        seq_t q_new = (seq_t)c * q + q_prev;
        q_prev = q;
        q = q_new;
    }
}
