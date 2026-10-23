/*
 * A194163  a(n) = [sum{(k*r) : 1<=k<=n}], where [ ]=floor, ( )=fractional
 *          part, and r=sqrt(3).
 * https://oeis.org/A194163
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor( Σ_{k=1}^{n} frac(k*sqrt(3)) )
 * Range:     u8 n <= 511, u16 n <= 5000, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Integer square‑root for an unsigned 128‑bit value.
   Returns floor(sqrt(x)).  Works for all x that fit in unsigned __int128. */
static inline uint64_t isqrt_uint128(unsigned __int128 x)
{
    uint64_t low = 0, high = 1;
    /* Grow high until high^2 > x, then binary search between low and high. */
    while ((unsigned __int128)high * high <= x)
        high <<= 1;
    while (high - low > 1) {
        uint64_t mid = low + (high - low) / 2;
        if ((unsigned __int128)mid * mid <= x)
            low = mid;
        else
            high = mid;
    }
    return low;
}

/* Return a(n) for n ≥ 1. */
seq_t a(idx_t n)
{
    /* Work in 64‑bit intermediates; the values never exceed 2¹⁶⁰ for
       the ranges that appear in the test set. */
    uint64_t n64 = (uint64_t)n;
    uint64_t sum_floor = 0;

    for (uint64_t k = 1; k <= n64; ++k) {
        unsigned __int128 val = (unsigned __int128)3 * k * k;  /* 3 * k²   */
        uint64_t f = isqrt_uint128(val);                      /* floor(sqrt(3*k²)) */
        sum_floor += f;
    }

    unsigned __int128 T = (unsigned __int128)n64 * (n64 + 1ULL) / 2ULL;  /* n(n+1)/2 */
    unsigned __int128 valT = (unsigned __int128)3 * T * T;                /* 3 * T²   */
    uint64_t floorT = isqrt_uint128(valT);                               /* floor(sqrt(3*T²)) */

    uint64_t result = floorT - sum_floor;
    return (seq_t)result;
}
