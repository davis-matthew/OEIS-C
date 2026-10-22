/*
 * A006705  Solution to Pellian: y such that x^2 - n y^2 = +- 1, +- 4.
 * https://oeis.org/A006705
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   continued-fraction search. a(n) = 0 if n is a square. Otherwise
 *            a(n) is the least y > 0 with x^2 - n*y^2 in {+-1, +-4}, using
 *            (X^2 - D*q^2) = (-1)^k*Q0*Q_k for the expansion of
 *            (P0+sqrt(D))/Q0: n == 2,3 (mod 4): y of the fundamental solution
 *            of x^2 - n y^2 = +-1; n == 0 (mod 4): y of x^2 - (n/4)...
 * Range:     u8 n <= 30, u16 n <= 93, u32 n <= 210, u64 n <= 630,
 *            u128 n <= 2010, big any n
 * Helpers:   a000037_isqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/a000037_isqrt.h"

/* Expand (P0 + sqrt(D))/Q0 as a continued fraction and return the
   denominator q_{k-1} of the first convergent whose complete-quotient
   denominator Q_k equals target. */
static seq_t cf_y(uint64_t D, int64_t P0, int64_t Q0, int64_t target)
{
    int64_t s = (int64_t)a000037_isqrt(D);
    int64_t P = P0, Q = Q0;
    seq_t q_prev = 0, q = 1;
    int64_t a0 = (P + s) / Q;
    (void)a0;
    int64_t ak = (P + s) / Q;
    bool first = true;
    while (1) {
        if (!first) {
            seq_t qn = (seq_t)ak * q + q_prev;
            q_prev = q;
            q = qn;
        }
        first = false;
        P = ak * Q - P;
        Q = ((int64_t)D - P * P) / Q;
        if (Q == target) return q;
        ak = (P + s) / Q;
    }
}

seq_t a(idx_t n)
{
    uint64_t s = a000037_isqrt((uint64_t)n);
    if (s * s == (uint64_t)n) return 0;

    uint64_t r = (uint64_t)(n % 4);
    if (r == 2 || r == 3) {
        return cf_y((uint64_t)n, 0, 1, 1);
    }
    if (r == 0) {
        seq_t y1 = cf_y((uint64_t)(n / 4), 0, 1, 1);
        if (y1 % 2 != 0) return y1;
        return cf_y((uint64_t)n, 0, 1, 1);
    }
    /* n == 1 (mod 4) */
    {
        seq_t y = cf_y((uint64_t)n, 1, 2, 2);
        if (y % 2 != 0) return y;
        return cf_y((uint64_t)n, 0, 1, 1);
    }
}
