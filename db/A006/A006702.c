/*
 * A006702  Solution to a Pellian equation: least x such that x^2 - n*y^2 = +-
 *          1.
 * https://oeis.org/A006702
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, nice, easy
 * Formula:   a(n) = p_{r-1}, numerator of the last convergent before the end
 *            of the first period of the continued fraction of sqrt(n) (r =
 *            period length); a(n) = 1 if n is a square. Convergents: p(-1) =
 *            1, p(0) = floor(sqrt(n)), p(k) = a_k*p(k-1) + p(k-2); stop when
 *            the complete-quotient denominator d becomes 1
 * Range:     u8 n <= 30, u16 n <= 93, u32 n <= 198, u64 n <= 525,
 *            u128 n <= 1725, big any n
 * Helpers:   isqrt_u64_local
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/isqrt_u64_local.h"

seq_t a(idx_t n_idx) {
    uint64_t n = (uint64_t)n_idx;
    uint64_t a0 = isqrt_u64_local(n);
    uint64_t m = 0, d = 1, ak = a0;
    seq_t p_prev, p, p_next;

    if (a0 * a0 == n) return (seq_t)1;

    p_prev = (seq_t)1;
    p = (seq_t)a0;
    for (;;) {
        m = d * ak - m;
        d = (n - m * m) / d;
        if (d == 1) return p;
        ak = (a0 + m) / d;
        p_next = p * ak + p_prev;
        p_prev = p;
        p = p_next;
    }
}
