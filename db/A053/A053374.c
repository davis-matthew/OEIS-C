/*
 * A053374  Write fundamental unit for real quadratic field of discriminant n
 *          as x + y*omega; sequence gives values of y for n == 2 mod 4.
 * https://oeis.org/A053374
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   a(n) = y, where x + y*sqrt(d) is the fundamental unit of
 *            Q(sqrt(d)) (x^2 - d*y^2 = +-1, smallest), d = A039956(n+1);
 *            computed from the continued fraction of sqrt(d)
 * Range:     u8 n <= 9, u16 n <= 18, u32 n <= 43, u64 n <= 56, u128 n <= 56,
 *            big any n
 * Validated: partial, pass, 57 terms through n = 56 (2026-11-07)
 * Helpers:   is_squarefree_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>
#include "../utils/is_squarefree_u64.h"

static uint64_t A053374_nth_d(uint64_t n)
{
    uint64_t cnt = 0, d;
    for (d = 2;; d += 4) {
        if (is_squarefree_u64(d / 2)) {
            if (cnt == n) return d;
            cnt++;
        }
    }
}

static uint64_t A053374_isqrt(uint64_t d)
{
    uint64_t r = 0, b = (uint64_t)1 << 31;
    while (b) {
        uint64_t t = r + b;
        if (t * t <= d) r = t;
        b >>= 1;
    }
    return r;
}

seq_t a(idx_t n)
{
    uint64_t d = A053374_nth_d((uint64_t)n);
    uint64_t a0 = A053374_isqrt(d);
    uint64_t m = 0, dd = 1, t = a0;
    seq_t pp = 1, p = (seq_t)a0, qp = 0, q = 1;
    for (;;) {
        m = t * dd - m;
        dd = (d - m * m) / dd;
        t = (a0 + m) / dd;
        if (t == 2 * a0) break;
        seq_t np = (seq_t)t * p + pp;
        seq_t nq = (seq_t)t * q + qp;
        pp = p; p = np;
        qp = q; q = nq;
    }
    return q;
}
