/*
 * A184913  n+[rn/s]+[tn/s]+[un/s], where []=floor and r=2^(1/5), s=r^2,
 *          t=r^3, u=r^4.
 * https://oeis.org/A184913
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n + ⌊n/r⌋ + ⌊n·r⌋ + ⌊n·r²⌋ (r = 2^(1/5))
 * Range:     u8 n <= 59, u16 n <= 120, u32 n <= 120, u64 n <= 120,
 *            u128 n <= 120, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Multi‑precision 5th power (fits in 128 bits for the ranges used). */
static inline __uint128_t pow5(__uint128_t x)
{
    return x * x * x * x * x;
}

/* Integer 5th root: floor(x^(1/5)).  Works for 0 ≤ x ≤ 4·10²⁰. */
static inline __uint128_t root5_floor(__uint128_t x)
{
    __uint128_t lo = 0, hi = 1;
    /* Find an upper bound (doubling) */
    while (pow5(hi) <= x) hi <<= 1;
    /* Narrow the interval */
    while (lo + 1 < hi) {
        __uint128_t mid = (lo + hi) >> 1;
        if (pow5(mid) <= x)
            lo = mid;
        else
            hi = mid;
    }
    return lo;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;                 /* safety guard (offset = 1) */

    /* Compute n⁵ once (fits into 128 bits for n ≤ 10⁶). */
    __uint128_t n128 = n;
    __uint128_t n5 = n128 * n128 * n128 * n128 * n128;

    /* floor(n/r)  = floor((n⁵/2)^(1/5))  */
    __uint128_t f1 = root5_floor(n5 / 2);

    /* floor(n·r)  = floor((n⁵·2)^(1/5))  */
    __uint128_t f2 = root5_floor(n5 * 2);

    /* floor(n·r²) = floor((n⁵·4)^(1/5))  */
    __uint128_t f3 = root5_floor(n5 * 4);

    __uint128_t result = n128 + f1 + f2 + f3;
    return (seq_t)result;                 /* implicit conversion to seq_t */
}
