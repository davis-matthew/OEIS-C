/*
 * A189362  a(n) = n + floor(n*r/s) + floor(n*t/s); r=1, s=sqrt(2), t=sqrt(3).
 * https://oeis.org/A189362
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n + floor(n/√2) + floor(n*√(3/2))
 * Range:     u8 n <= 87, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#ifndef _A189362_H_
#define _A189362_H_

/* Compute integer square root of a 128-bit unsigned integer:
   returns ⌊√x⌋.  */
static inline __uint128_t isqrt128(__uint128_t x)
{
    __uint128_t lo = 0;
    __uint128_t hi = (__uint128_t)1 << 64;  /* upper bound > sqrt(2^128-1) */

    while (lo + 1 < hi) {
        __uint128_t mid = (lo + hi) >> 1;
        if (mid <= x / mid)   /* mid*mid ≤ x without overflow */
            lo = mid;
        else
            hi = mid;
    }
    return lo;
}

seq_t a(idx_t n)
{
    /* Offset is 1; for n < 1 the behaviour is unspecified. */
    if (n == 0)
        return 0;

    __uint128_t n128 = ( __uint128_t ) n;

    /* floor(n * √2)  -> floor( sqrt( 2 * n^2 ) )   */
    __uint128_t v2  = n128 * n128 * 2;
    __uint128_t f2  = isqrt128(v2);

    /* floor(n * √6)  -> floor( sqrt( 6 * n^2 ) )   */
    __uint128_t v6  = n128 * n128 * 6;
    __uint128_t f6  = isqrt128(v6);

    /* floor(n/√2) = floor(floor(n√2)/2)  , similarly for √6 */
    seq_t term2 = (seq_t)(f2 >> 1);
    seq_t term3 = (seq_t)(f6 >> 1);

    /* a(n) = n + term2 + term3 */
    return (seq_t)n + term2 + term3;
}

#endif /* _A189362_H_ */
