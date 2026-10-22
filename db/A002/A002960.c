/*
 * A002960  The square sieve.
 * https://oeis.org/A002960
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1) + 1 + floor(sqrt(a(n-1) + 1 +
 *            floor(sqrt(a(n-1))))), a(1) = 2
 * Range:     u8 n <= 26, u16 n <= 502, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   a002960_raise_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/a002960_raise_sqrt.h"

seq_t a(idx_t n) {
    seq_t cur = (seq_t)2;
    seq_t s = (seq_t)1; /* floor(sqrt(cur)) */
    idx_t i;
    for (i = 2; i <= n; i++) {
        seq_t t, r;
        s = a002960_raise_sqrt(s, cur);
        t = cur + 1 + s;
        r = a002960_raise_sqrt(s, t);
        cur = cur + 1 + r;
        s = r; /* r <= floor(sqrt(new cur)) since new cur >= t */
    }
    return cur;
}
