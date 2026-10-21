/*
 * A227041  Triangle of numerators of harmonic mean of n and m, 1 <= m <= n.
 * https://oeis.org/A227041
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, frac, tabl
 * Formula:   a(n) = 2*r*m / gcd(r+m, 2*r*m), with r =
 *            floor((sqrt(8*k+1)-1)/2)+1, k = n-1, and m = k - (r-1)*r/2 + 1
 * Range:     u8 n <= 66, u16 n <= 66, u32 n <= 66, u64 n <= 66, u128 n <= 66,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/gcd.h"

/* Integer square root of a 64‑bit unsigned integer using 128‑bit arithmetic. */
static inline uint64_t isqrt_u64(uint64_t n)
{
    uint64_t lo = 0, hi = UINT64_MAX;
    while (lo <= hi)
    {
        uint64_t mid = lo + ((hi - lo) >> 1);
        unsigned __int128 sq = (unsigned __int128)mid * mid;
        if (sq <= n)
            lo = mid + 1;
        else
            hi = mid - 1;
    }
    return hi;
}

/* Integer square root of a 128‑bit unsigned integer, returns 64‑bit root. */
static inline uint64_t isqrt_u128(__int128 n)
{
    uint64_t lo = 0, hi = UINT64_MAX;
    while (lo <= hi)
    {
        uint64_t mid = lo + ((hi - lo) >> 1);
        __int128 sq = (__int128)mid * mid;
        if (sq <= n)
            lo = mid + 1;
        else
            hi = mid - 1;
    }
    return hi;
}

/* Return the nth term of the sequence (1‑based). */
seq_t a(idx_t n)
{
    /* Convert 1‑based index to zero‑based. */
    uint64_t k = (uint64_t)(n - 1);          /* k may be zero */

    /* Compute r by solving k < r(r+1)/2 <= k+1. */
    __int128 d128 = (__int128)8 * k + 1;
    uint64_t sqrt_d = isqrt_u128(d128);
    uint64_t r = (sqrt_d + 1) / 2;          /* r >= 1 */

    /* Column position within the row. */
    uint64_t start = (r * (r - 1)) / 2;
    uint64_t m0 = k - start;                /* zero‑based column index */
    uint64_t m = m0 + 1;                    /* 1‑based column index */

    seq_t rn = (seq_t)r;
    seq_t mm = (seq_t)m;

    /* Numerator of the harmonic mean. */
    seq_t numer = (seq_t)2 * rn * mm;

    /* gcd of (r+m) and the numerator. */
    seq_t g = gcd(rn + mm, numer);

    return numer / g;
}
