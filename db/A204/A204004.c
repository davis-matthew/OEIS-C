/*
 * A204004  Symmetric matrix based on f(i,j) = max{2i+j-2,i+2j-2}, by
 *          antidiagonals.
 * https://oeis.org/A204004
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = 2*(t+1) + max{ t*(t+1)/2 - n, * n - (t*t+3*t+4)/2 }, *
 *            where t = ⌊( -1 + √(8*n-7) )/2⌋. * This is the definition of
 *            OEIS A204004 (m = 2)
 * Range:     u8 n <= 72, u16 n <= 72, u32 n <= 72, u64 n <= 72, u128 n <= 72,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>


/* Integer square‑root for a non‑negative 128‑bit integer. */
static inline uint64_t isqrt_uint128(__uint128_t x)
{
    /* Upper bound: sqrt(2^67) < 2^34.  Using 1 << 34 gives a safe limit. */
    uint64_t lo = 0, hi = (uint64_t)1 << 34;
    while (lo < hi) {
        uint64_t mid = (lo + hi + 1) >> 1;
        __uint128_t mid2 = (__uint128_t)mid * mid;
        if (mid2 <= x)
            lo = mid;
        else
            hi = mid - 1;
    }
    return lo;
}

seq_t a(idx_t n)
{
    /* n is guaranteed >= 1 (offset 1).  Compute t. */
    __uint128_t val = (__uint128_t)8 * n - 7;   /* 8*n-7, fits in 128 bits */
    uint64_t sqrtval = isqrt_uint128(val);
    uint64_t t64 = (sqrtval - 1) >> 1;           /* floor((sqrt-1)/2) */
    __int128 t   = (__int128)t64;
    __int128 n128 = (__int128)n;

    /* t*(t+1)/2 and (t*t+3*t+4)/2 */
    __int128 A = t * (t + 1) / 2;
    __int128 B = (t * t + 3 * t + 4) / 2;

    /* Differences, may be negative */
    __int128 diff1 = A - n128;
    __int128 diff2 = n128 - B;
    __int128 maxdiff = diff1 > diff2 ? diff1 : diff2;

    /* Final result, guaranteed non‑negative */
    __int128 result = 2 * (t + 1) + maxdiff;
    return (seq_t)result;
}
