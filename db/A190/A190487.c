/*
 * A190487  a(n) = [(bn+c)r]-b[nr]-[cr], where (r,b,c)=(sqrt(2),3,0) and
 *          []=floor.
 * https://oeis.org/A190487
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor(3*n*sqrt(2)) - 3*floor(n*sqrt(2))
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* seq_t and idx_t are defined in the build environment. */

#include <stdint.h>

/* Integer square root for unsigned __int128 arguments.
   Returns floor(sqrt(x)). */
static inline idx_t isqrt_u128(__uint128_t x)
{
    /* The root of a 128-bit number fits in at most 64 bits. */
    idx_t lo = 0;
    idx_t hi = (idx_t)1 << 63;          /* 2^63 is more than enough for all n used */
    while (lo < hi) {
        idx_t mid = lo + ((unsigned long long)(hi - lo + 1) >> 1);
        if ((__uint128_t)mid * mid <= x)
            lo = mid;
        else
            hi = mid - 1;
    }
    return lo;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;              /* safety for indices below offset */

    /* Compute floor(n*sqrt(2)) as floor(sqrt(2*n^2)). */
    __uint128_t prod1 = (__uint128_t)2 * n * n;
    idx_t r1 = isqrt_u128(prod1);

    /* Compute floor(3*n*sqrt(2)) as floor(sqrt(18*n^2)). */
    __uint128_t prod2 = (__uint128_t)18 * n * n;
    idx_t r2 = isqrt_u128(prod2);

    return (seq_t)r2 - (seq_t)(3 * r1);
}
