/*
 * A195169  a(n) = 3*n - floor(2*n*r/3), where r=(1+sqrt(5))/2 (the golden
 *          ratio).
 * https://oeis.org/A195169
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 3*n - floor(2*n*r/3), where r = (1+sqrt(5))/2
 * Range:     u8 n <= 131, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Simplified: a(n) = 3*n - floor(n*(1+√5)/3) = 3*n - floor((n + floor(n*√5))/3) */

#include <stdint.h>

static inline __int128 isqrt128(__int128 x)
{
    if (x <= 0) return 0;
    __int128 lo = 0;
    __int128 hi = (__int128)1;
    /* find an upper bound hi >= sqrt(x) */
    while (hi * hi <= x) hi <<= 1;
    __int128 res = 0;
    while (lo <= hi) {
        __int128 mid = (lo + hi) >> 1;
        if (mid > x / mid) {
            hi = mid - 1;
        } else {
            res = mid;
            lo = mid + 1;
        }
    }
    return res;
}

seq_t a(idx_t n)
{
    /* n fits in idx_t (uint64_t). All intermediate values are small enough for __int128. */
    __int128 nn = (__int128)n * (__int128)n;        /* n^2          */
    __int128 xx = (__int128)5 * nn;                 /* 5*n^2        */
    __int128 k = isqrt128(xx);                      /* floor(n*√5)  */
    seq_t d = (seq_t)(( (seq_t)n + (seq_t)k ) / 3);  /* floor((n+k)/3) */
    seq_t res = (seq_t)3 * n - d;                   /* a(n)         */
    return res;
}
