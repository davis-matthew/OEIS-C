/*
 * A213088  The Manhattan distance to the origin while traversing the first
 *          quadrant in a taxicab geometry.
 * https://oeis.org/A213088
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, walk
 * Formula:   a(n) = t + abs(t^2 - n) - 1 with t = floor(sqrt(n)+1/2)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Integer square root: floor(sqrt(n)) for 0 <= n < 2^64 */
static inline idx_t isqrt_u64(idx_t n)
{
    idx_t lo = 0;
    idx_t hi = (idx_t)1 << 32;          /* 2^32 is an upper bound for sqrt(2^64-1) */
    while (lo < hi) {
        idx_t mid = (lo + hi + 1) >> 1;
        __uint128_t m2 = (__uint128_t)mid * mid;
        if (m2 <= n)
            lo = mid;
        else
            hi = mid - 1;
    }
    return lo;
}

seq_t a(idx_t n)
{
    /* n >= 1 (offset 1) */
    idx_t s = isqrt_u64(n);                    /* floor(sqrt(n))          */
    idx_t diff = n - s * s;                    /* n - s^2, 0 <= diff < 2*s+1 */
    /* t = nearest integer to sqrt(n) */
    idx_t t = s + ((diff > s) ? 1 : 0);

    __uint128_t tt = (__uint128_t)t * t;        /* t^2, may overflow uint64_t */
    idx_t d2;
    if (tt > n)
        d2 = (idx_t)(tt - n);
    else
        d2 = (idx_t)(n - tt);

    return (seq_t)(t + d2 - 1);
}
