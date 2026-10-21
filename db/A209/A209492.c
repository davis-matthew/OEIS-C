/*
 * A209492  a(0)=1; for n >= 1, let k = floor((1 + sqrt(8*n-7))/2), m = n -
 *          (k^2 - k+2)/2. Then a(n) = 2^k + 2^(m+1) - 1.
 * https://oeis.org/A209492
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, tabl
 * Formula:   a(n) = 2^k + 2^(m+1) - 1, where k = floor((1 + sqrt(8*n-7))/2)
 *            (k >= 1 for n >= 1) and m = n - (k^2 - k + 2)/2. a(0) = 1
 * Range:     u8 n <= 28, u16 n <= 120, u32 n <= 496, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/pow2.h"

/* Integer square root of a 128‑bit unsigned integer.
   Returns floor(sqrt(x)). */
static idx_t isqrt_u128(unsigned __int128 x)
{
    idx_t lo = 0, hi = ~(idx_t)0;   /* 2^64-1 */
    while (lo <= hi) {
        idx_t mid = lo + ((hi - lo) >> 1);
        unsigned __int128 sq = (unsigned __int128)mid * mid;
        if (sq <= x)
            lo = mid + 1;
        else
            hi = mid - 1;
    }
    return lo - 1;
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    if (n == 0u)
        return (seq_t)1;

    /* Compute k = floor((1 + sqrt(8*n-7))/2) without overflow. */
    unsigned __int128 tmp128 = (unsigned __int128)n * 8 - 7;
    idx_t sqrt_val = isqrt_u128(tmp128);
    idx_t k = (sqrt_val + 1) >> 1;

    /* base = (k^2 - k + 2)/2   (same as k*(k-1)/2 + 1) */
    unsigned __int128 base128 = ((unsigned __int128)k * (k - 1)) >> 1;
    base128 += 1;
    idx_t base = (idx_t)base128;

    idx_t m = n - base;

    seq_t part1 = pow2(k);          /* 2^k */
    seq_t part2 = pow2(m + 1);      /* 2^(m+1) */

    return part1 + part2 - (seq_t)1;
}
