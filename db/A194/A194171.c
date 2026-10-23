/*
 * A194171  [sum{(k*r) : 1<=k<=n}], where [ ]=floor, ( )=fractional part, and
 *          r=1/2+sqrt(5).
 * https://oeis.org/A194171
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor( sum_{k=1}^n frac(k * (1/2 + sqrt5)) )
 * Range:     u8 n <= 72, u16 n <= 72, u32 n <= 72, u64 n <= 72, u128 n <= 72,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static inline __uint128_t isqrt128(__uint128_t x)
{
    /* integer square root of x: floor(sqrt(x)) */
    if (x == 0) return 0;
    __uint128_t lo = 0;
    __uint128_t hi = ((__uint128_t)1 << 64) - 1;   /* hi < 2^64, hi^2 < 2^128 */
    __uint128_t ans = 0;
    while (lo <= hi) {
        __uint128_t mid = (lo + hi) >> 1;
        __uint128_t sq = mid * mid;
        if (sq <= x) {
            ans = mid;
            lo = mid + 1;
        } else {
            hi = mid - 1;
        }
    }
    return ans;
}

seq_t a(idx_t n)
{
    /* basic types */
    __uint128_t nn = n;
    /* triangular number T = n*(n+1)/2 */
    __uint128_t T = nn * (nn + 1) / 2;

    /* sum of floor(k*r) for k=1..n */
    __uint128_t sum_floor = 0;
    for (idx_t k = 1; k <= n; ++k) {
        __uint128_t kk = k;
        __uint128_t temp = 20 * kk * kk;                     /* 20*k^2 */
        __uint128_t sqrt20k2 = isqrt128(temp);              /* floor(sqrt(20*k^2)) */
        __uint128_t floor_val = (kk + sqrt20k2) / 2;         /* floor(k*(1/2+sqrt5)) */
        sum_floor += floor_val;
    }

    /* floor(T*sqrt5) and floor(T*sqrt20)  */
    __uint128_t A128 = isqrt128(5 * T * T);                 /* floor(T*sqrt5)  */
    __uint128_t C128 = isqrt128(20 * T * T);                /* floor(T*sqrt20) */

    /* B_half = 0 if frac(T*sqrt5) < 0.5, otherwise 1 */
    uint64_t B_half = (uint64_t)(C128 - 2 * A128);

    /* Y = A - sum_floor + floor(T/2) */
    __uint128_t Y128 = A128 - sum_floor + (T / 2);

    /* final result */
    __uint128_t res128 = Y128 + ((T & 1) && B_half ? 1 : 0);

    return (seq_t)res128;
}
