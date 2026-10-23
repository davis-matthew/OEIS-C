/*
 * A184733  a(n) = floor(n*s+h-h*s), where s=(3+sqrt(5))/2, h=-1/4; complement
 *          of A184732.
 * https://oeis.org/A184733
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor((12*n+1 + (4*n+1)*sqrt(5))/8)
 * Range:     u8 n <= 97, u16 n <= 120, u32 n <= 120, u64 n <= 120,
 *            u128 n <= 120, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Integer square‑root of a 128‑bit unsigned integer.
   Returns floor(sqrt(x)).  Works for all input values that fit in __uint128_t. */
static idx_t isqrt_u128(__uint128_t x)
{
    idx_t lo = 0, hi = (idx_t)~0ull;          /* upper bound: 2^64-1 */
    while (lo <= hi)
    {
        idx_t mid = lo + ((hi - lo) >> 1);
        __uint128_t sq = (__uint128_t)mid * mid;
        if (sq == x)
            return mid;
        if (sq < x)
            lo = mid + 1;
        else
            hi = mid - 1;
    }
    return hi;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;                    /* safety for 0‑index, not used */

    /* Compute the terms using 128‑bit arithmetic to avoid overflow. */
    __uint128_t A = (__uint128_t)12 * n + 1;   /* 12*n + 1            */
    __uint128_t B = (__uint128_t)4 * n  + 1;   /* 4*n + 1             */
    __uint128_t prod = (__uint128_t)5 * B * B; /* 5*B*B = (B*sqrt5)^2 */

    /* floor(B*sqrt5) = floor(sqrt(5*B*B)) */
    idx_t t = isqrt_u128(prod);

    /* numerator = A + floor(B*sqrt5) */
    __uint128_t numerator = A + t;

    /* a(n) = floor((A + B*sqrt5)/8) = (A + floor(B*sqrt5)) / 8 */
    __uint128_t result128 = numerator / 8;

    return (seq_t)result128;
}
