/*
 * A013610  Triangle of coefficients in expansion of (1+3*x)^n.
 * https://oeis.org/A013610
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  tabl, nonn, easy
 * Formula:   a(n) = 3^k * binomial(r, k), * where r =
 *            floor((sqrt(8*n+1)-1)/2) and k = n - r*(r+1)/2
 * Range:     u8 n <= 17, u16 n <= 51, u32 n <= 181, u64 n <= 617,
 *            u128 n <= 2256, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static idx_t isqrt_u64(idx_t x)
{
    idx_t lo = 0, hi = x, mid, res = 0;
    while (lo <= hi)
    {
        mid = lo + ((hi - lo) >> 1);
        if ((unsigned __int128)mid * mid <= x)
        {
            res = mid;
            lo = mid + 1;
        }
        else
        {
            if (mid == 0) break;
            hi = mid - 1;
        }
    }
    return res;
}

seq_t a(idx_t n)
{
    /* Find row r such that r*(r+1)/2 <= n < (r+1)*(r+2)/2 */
    idx_t t = 8 * n + 1;
    idx_t sq = isqrt_u64(t);
    idx_t r = (sq - 1) >> 1;            /* floor((sqrt(8*n+1)-1)/2) */

    idx_t start = r * (r + 1) / 2;      /* first index of row r */
    idx_t k = n - start;                /* column within row */

    /* Compute binomial(r, k) multiplicatively */
    seq_t bin = 1;
    idx_t kk = k;
    for (idx_t i = 1; i <= kk; ++i)
    {
        bin = bin * (r - kk + i) / i;
    }

    /* Compute 3^k */
    seq_t p3 = 1;
    for (idx_t i = 0; i < kk; ++i)
        p3 *= 3;

    return bin * p3;
}
