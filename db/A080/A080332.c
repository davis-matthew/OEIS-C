/*
 * A080332  G.f.: Product_{n>0} (1 - x^n)^3 * (1 - x^(2*n - 1))^2 = Sum_{n in
 *          Z} (6*n + 1) * x^(n*(3*n + 1)/2).
 * https://oeis.org/A080332
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = 6*k + 1 where n = k*(3*k + 1)/2 for some integer k; if no
 *            such k exists, a(n) = 0
 * Range:     i8 n <= 714, i16 n <= 10000, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static inline unsigned __int128 isqrt128(unsigned __int128 x) {
    unsigned __int128 lo = 0, hi = x;
    while (lo <= hi) {
        unsigned __int128 mid = lo + ((hi - lo) >> 1);
        unsigned __int128 sq  = mid * mid;
        if (sq == x) return mid;
        if (sq < x) {
            lo = mid + 1;
        } else {
            if (mid == 0) break;
            hi = mid - 1;
        }
    }
    return lo - 1;   /* floor of sqrt(x) */
}

seq_t a(idx_t n) {
    /* D = 1 + 24*n  (using 128‑bit arithmetic to avoid overflow) */
    unsigned __int128 D = (unsigned __int128)1 + (unsigned __int128)24 * (unsigned __int128)n;
    unsigned __int128 r = isqrt128(D);
    if (r * r != D) return 0;          /* not a generalized pentagonal number */

    /* try positive k: k = (r - 1) / 6 */
    __int128 u = (__int128)r - 1;
    if (u >= 0 && u % 6 == 0) {
        __int128 k = u / 6;
        return (seq_t)(6 * k + 1);
    }

    /* try negative k: k = -(r + 1) / 6 */
    __int128 v = -((__int128)r + 1);
    if (v % 6 == 0) {
        __int128 k = v / 6;
        return (seq_t)(6 * k + 1);
    }

    return 0;   /* no integer k produces this index */
}
