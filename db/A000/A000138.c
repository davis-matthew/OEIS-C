/*
 * A000138  Expansion of e.g.f. exp(-x^4/4)/(1-x).
 * https://oeis.org/A000138
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n! * Sum_{i=0..floor(n/4)} (-1)^i / (i! * 4^i). Computed
 *            by recurrence: a(0) = 1; a(n) = n*a(n-1) if 4 does not divide n,
 *            and a(4m) = 4m*a(4m-1) + (-1)^m * t(m), where t(m) =
 *            (4m)!/(m!*4^m), t(0) = 1, t(m) = t(m-1)*(4m-1)*(4m-2)*(4m-3)
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 12, u64 n <= 20, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    seq_t r = 1;
    seq_t t = 1;
    idx_t k;
    for (k = 1; k <= n; k++) {
        r *= (seq_t)k;
        if (k % 4 == 0) {
            idx_t m = k / 4;
            t *= (seq_t)(k - 1);
            t *= (seq_t)(k - 2);
            t *= (seq_t)(k - 3);
            if (m & 1)
                r -= t;
            else
                r += t;
        }
    }
    return r;
}
