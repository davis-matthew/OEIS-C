/*
 * A090638  Duplicate of A073869.
 * https://oeis.org/A090638
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  dead
 * Formula:   a(n) = ⌊n*φ⌋ – n (φ = (1+√5)/2)
 * Range:     i8 n <= 29, i16 n <= 29, i32 n <= 29, i64 n <= 29, i128 n <= 29,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Integer square root for an unsigned __int128 value.
   Returned value is floor(√x).                                      */
static __uint128_t isqrt_uint128(__uint128_t x)
{
    __uint128_t lo = 0, hi = x, ans = 0;
    while (lo <= hi)
    {
        __uint128_t mid = (lo + hi) >> 1;
        if (mid == 0)                /* avoids division by zero */
        {
            ans = 0;
            lo = 1;
            continue;
        }
        if (mid > x / mid)           /* mid*mid > x  (no overflow) */
            hi = mid - 1;
        else
        {
            ans = mid;
            lo = mid + 1;
        }
    }
    return ans;
}

seq_t a(idx_t n)
{
    /* Compute floor(n*φ) = floor( (n + floor(n*√5)) / 2 )            */
    __uint128_t nn = (__uint128_t)n;                     /* n in 128‑bit      */
    __uint128_t prod = (__uint128_t)5 * nn * nn;          /* 5*n^2              */
    __uint128_t m    = isqrt_uint128(prod);              /* floor(n*√5)       */
    __uint128_t floor_phi = (nn + m) >> 1;                /* floor(n*φ)        */
    __uint128_t result = floor_phi - nn;                 /* a(n) = floor(n*φ) – n */
    return (seq_t)result;
}
