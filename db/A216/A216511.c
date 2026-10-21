/*
 * A216511  Number of positive integer solutions to the equation a^2 + 7*b^2 =
 *          n.
 * https://oeis.org/A216511
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Number of positive integer solutions to a^2 + 7*b^2 = n. */
#include <stdint.h>

/* Integer square‑root of a 64‑bit unsigned value.
   Returns floor(sqrt(x)).  Works for all values that fit in idx_t. */
static inline idx_t isqrt_u64(idx_t x)
{
    idx_t lo = 0;
    idx_t hi = 0xFFFFFFFFULL;   /* sqrt(2^64-1) = 2^32-1 */
    idx_t ans = 0;

    while (lo <= hi)
    {
        idx_t mid = lo + ((hi - lo) >> 1);
        if (mid == 0)
        {
            if (x == 0) return 0;
            ans = 0;
            lo = 1;
            continue;
        }
        /* avoid overflowing mid*mid by using division */
        if (mid > x / mid)
        {
            hi = mid - 1;
        }
        else
        {
            ans = mid;
            lo = mid + 1;
        }
    }
    return ans;
}

seq_t a(idx_t n)
{
    seq_t count = 0;

    if (n == 0)
        return 0;

    /* maximum b such that 7*b^2 <= n */
    idx_t max_b = isqrt_u64(n / 7);

    for (idx_t b = 1; b <= max_b; ++b)
    {
        idx_t term = 7ULL * b * b;   /* 7*b^2 */
        if (term > n)                /* safety guard */
            break;

        idx_t rem = n - term;        /* a^2 */
        if (rem == 0)                /* a would be 0, not allowed */
            continue;

        idx_t a_val = isqrt_u64(rem);

        if (a_val * a_val == rem)    /* perfect square check */
            ++count;
    }

    return count;
}
