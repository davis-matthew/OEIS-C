/*
 * A045930  The generalized Connell sequence C_{3,5}.
 * https://oeis.org/A045930
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3*n - 2*floor((13 + sqrt(40*n-31))/10)
 * Range:     u8 n <= 64, u16 n <= 64, u32 n <= 64, u64 n <= 64, u128 n <= 64,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Integer square‑root: floor(sqrt(x)) for unsigned 64–bit x.
   Uses binary search with 128‑bit multiplication to avoid overflow. */
static idx_t isqrt_u64(idx_t x)
{
    idx_t lo = 0, hi = (idx_t)1 << 32;      /* sqrt(UINT64_MAX) < 2^32 */
    while (lo <= hi) {
        idx_t mid = lo + ((hi - lo) >> 1);
        __uint128_t sq = (__uint128_t)mid * ( __uint128_t)mid;
        if (sq <= x)
            lo = mid + 1;
        else
            hi = mid - 1;
    }
    return hi;   /* hi is the largest integer with hi^2 <= x */
}

seq_t a(idx_t n)
{
    /* Compute 40*n - 31 as an unsigned 64‑bit value. */
    idx_t val = (idx_t)40 * n - 31;

    /* floor(sqrt(val)) */
    idx_t sqrt_val = isqrt_u64(val);

    /* floor((13 + sqrt_val) / 10) */
    idx_t part = (13 + sqrt_val) / 10;

    /* 3*n in seq_t domain */
    seq_t ans = (seq_t)3 * (seq_t)n;

    /* subtract 2*part */
    ans -= (seq_t)(2 * part);

    return ans;
}
