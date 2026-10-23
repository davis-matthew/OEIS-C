/*
 * A185916  Weight array of A185914, by antidiagonals.
 * https://oeis.org/A185916
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  tabl, sign
 * Formula:   a(n) = 1 if n = 1 + (m-1)m/2 for some integer m ≥ 1, a(n) = -1
 *            if n = 2m^2 + 1 for some integer m ≥ 1, a(n) = 0 otherwise
 * Range:     i8 n <= 1275, i16 n <= 1275, i32 n <= 1275, i64 n <= 1275,
 *            i128 n <= 1275, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* integer square root: returns floor(sqrt(x)) for 0 ≤ x ≤ 2^64-1 */
static idx_t isqrt_u64(idx_t x) {
    if (x < 2) return x;
    idx_t lo = 0, hi = (x >> 1) + 1;
    while (lo < hi) {
        idx_t mid = (lo + hi + 1) >> 1;
        __int128 sq = (__int128)mid * mid;
        if (sq <= x)
            lo = mid;
        else
            hi = mid - 1;
    }
    return lo;
}

seq_t a(idx_t n) {
    if (n < 1) return (seq_t)0;

    /* Check for -1: n = 2m^2 + 1 with m ≥ 1 */
    if (n >= 3) {
        idx_t x = n - 1;
        if ((x & 1) == 0) {
            idx_t y = x >> 1;                 /* y = m^2 */
            idx_t m = isqrt_u64(y);
            if (m > 0 && (__int128)2 * m * m + 1 == n)
                return (seq_t)-1;
        }
    }

    /* Check for 1: n = 1 + (m-1)m/2  ->  n-1 = m(m-1)/2 */
    idx_t t = n - 1;
    idx_t d = 1 + (idx_t)((__int128)8 * t);   /* d = 1 + 8*(n-1) */
    idx_t sqrt_d = isqrt_u64(d);
    if ((__int128)sqrt_d * sqrt_d == d && ((1 + sqrt_d) & 1) == 0)
        return (seq_t)1;

    return (seq_t)0;
}
