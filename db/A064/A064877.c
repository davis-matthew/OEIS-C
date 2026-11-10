/*
 * A064877  a(n) = n - (A064873(n) + A064874(n) + A064875(n) + A064876(n)).
 * https://oeis.org/A064877
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n - (A064873(n)+A064874(n)+A064875(n)+A064876(n)), where
 *            the four terms are the lexicographically minimal sorted
 *            decomposition n = x^2+y^2+z^2+w^2, x<=y<=z<=w; computed by
 *            direct search
 * Range:     u8 n <= 73, u16 n <= 73, u32 n <= 73, u64 n <= 73, u128 n <= 73,
 *            big any n
 * Validated: partial, pass, 74 terms through n = 73 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static idx_t A064877_isqrt(idx_t v)
{
    idx_t lo = 0, hi = 1;
    while (hi <= v / hi) hi <<= 1;   /* hi*hi > v */
    while (lo + 1 < hi) {
        idx_t mid = lo + (hi - lo) / 2;
        if (mid <= v / mid) lo = mid; else hi = mid;
    }
    return lo;
}

seq_t a(idx_t n)
{
    for (idx_t x = 0; 4 * x * x <= n; ++x) {
        idx_t r1 = n - x * x;
        for (idx_t y = x; 3 * y * y <= r1; ++y) {
            idx_t r2 = r1 - y * y;
            for (idx_t z = y; 2 * z * z <= r2; ++z) {
                idx_t r3 = r2 - z * z;
                idx_t w = A064877_isqrt(r3);
                if (w * w == r3 && w >= z) {
                    return (seq_t)n - ((seq_t)x + (seq_t)y + (seq_t)z + (seq_t)w);
                }
            }
        }
    }
    return (seq_t)0;
}
