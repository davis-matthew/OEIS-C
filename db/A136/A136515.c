/*
 * A136515  Number of unit square lattice cells inside half-plane (two
 *          adjacent quadrants) of origin centered circle of diameter 2n+1.
 * https://oeis.org/A136515
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 2*Sum_{k=1..n} floor( sqrt((n+1/2)^2 - k^2) )
 * Range:     u8 n <= 12, u16 n <= 204, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Integer square root for a 128‑bit unsigned value.
   Returns the greatest integer s with s*s <= x. */
static idx_t isqrt_u128(unsigned __int128 x)
{
    unsigned __int128 lo = 0;
    unsigned __int128 hi = ((unsigned __int128)1) << 64;   /* 2^64 > sqrt(UINT128_MAX) */

    while (lo < hi) {
        unsigned __int128 mid = (lo + hi) >> 1;
        unsigned __int128 sq  = mid * mid;

        if (sq <= x)
            lo = mid + 1;
        else
            hi = mid;
    }
    /* lo is the first with lo^2 > x */
    return (idx_t)(lo - 1);
}

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;

    /* m = 2*n + 1,  m^2 = (2n+1)^2 */
    unsigned __int128 m  = (unsigned __int128)2 * (unsigned __int128)n + 1;
    unsigned __int128 m2 = m * m;

    seq_t sum_half = 0;   /* sum of floor( sqrt((n+1/2)^2 - k^2) ), k=1..n */

    for (idx_t k = 1; k <= n; ++k) {
        unsigned __int128 k2   = (unsigned __int128)k * (unsigned __int128)k;
        unsigned __int128 diff = m2 - (unsigned __int128)4 * k2;   /* (2n+1)^2 - 4k^2 */

        idx_t s = isqrt_u128(diff);       /* floor( sqrt(diff) ) */
        sum_half += (seq_t)(s >> 1);      /* floor( sqrt(diff)/2 ) */
    }

    return sum_half << 1;   /* multiply by 2 to obtain a(n) */
}
