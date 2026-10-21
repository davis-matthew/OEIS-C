/*
 * A236533  Number of non-congruent Heronian triangles with base length n
 *          whose apex lies on or within a space bounded by a semicircle of
 *          diameter n.
 * https://oeis.org/A236533
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of unordered Heronian triangles with base length n
 *            * whose apex lies on or within the semicircle of diameter n.
 *            Equivalently, * count integer pairs (a,b) with a ≤ b < n, a + b
 *            > n, a² + b² ≤ n², and * (a+b+n)(-a+b+n)(a-b+n)(a+b-n) is a
 *            perfect square
 * Range:     u8 n <= 85, u16 n <= 85, u32 n <= 85, u64 n <= 85, u128 n <= 85,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Integer square root for uint64_t using binary search.  Returns floor(sqrt(x)). */
static inline uint64_t isqrt_u64(uint64_t x)
{
    uint64_t lo = 0, hi = 400000000ULL;              /* 4e8 > sqrt(1.6·10^17) */
    while (lo <= hi)
    {
        uint64_t mid = lo + ((hi - lo) >> 1);
        __int128 sq = (__int128)mid * mid;
        if (sq <= x)
            lo = mid + 1;
        else
            hi = mid - 1;
    }
    return hi;
}

seq_t a(idx_t n)
{
    seq_t result = 0;
    if (n < 5) return result;                     /* trivially zero */

    idx_t nVal = n;
    idx_t nSq  = nVal * nVal;

    /* maximum a is floor(sqrt(n²/2)) */
    idx_t aMax = isqrt_u64(nSq / 2);

    for (idx_t a = 1; a <= aMax; ++a)
    {
        idx_t aSq = a * a;

        /* lower bound for b: max(a, n-a+1) */
        idx_t lowerB = nVal - a + 1;
        if (lowerB < a) lowerB = a;

        /* upper bound for b */
        idx_t maxB1 = nVal - 1;                     /* because b < n */
        idx_t maxB2 = isqrt_u64(nSq - aSq);        /* sqrt(n² - a²) */
        idx_t maxB = (maxB1 < maxB2) ? maxB1 : maxB2;
        if (lowerB > maxB) continue;

        for (idx_t b = lowerB; b <= maxB; ++b)
        {
            /* Heron product components */
            uint64_t t1 = a + b + nVal;
            uint64_t t2 = nVal + b - a;
            uint64_t t3 = nVal + a - b;
            uint64_t t4 = a + b - nVal;

            /* product fits in 64 bits (<1.6e17 for n ≤ 10000) */
            unsigned __int128 prod = (unsigned __int128)t1 * t2 * t3 * t4;
            uint64_t h = (uint64_t)prod;

            uint64_t r = isqrt_u64(h);
            if ((unsigned __int128)r * r == h)
                result += 1;
        }
    }

    return result;
}
