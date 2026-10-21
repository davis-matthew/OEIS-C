/*
 * A217840  Total number of solutions to the equation x^2 + k*y^2 = n with x
 *          >= 0, y >= 0, k > 0, or 0 if the number is infinite. Order matters
 *          for the equation x^2 + y^2 = n.
 * https://oeis.org/A217840
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 0 if n is a square, otherwise a(n) = number of solutions
 *            to x^2 + k*y^2 = n with x>=0, y>=0, k>0. This is computed by
 *            looping over x and y and checking (n - x^2) % y^2 == 0
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

/* Integer square root for 64‑bit unsigned values */
static inline idx_t floor_sqrt_u64(idx_t n)
{
    idx_t low = 0;
    idx_t high = (idx_t)1 << 32;          /* sqrt(UINT64_MAX) < 2^32 */
    while (low < high)
    {
        idx_t mid = low + ((high - low + 1) >> 1);
        if (mid > n / mid)
            high = mid - 1;
        else
            low  = mid;
    }
    return low;
}

seq_t a(idx_t n)
{
    /* Check for square: infinite number of solutions → return 0 */
    idx_t sq = floor_sqrt_u64(n);
    if (sq * sq == n)
        return 0;

    seq_t count = 0;

    for (idx_t x = 0; x <= sq; ++x)
    {
        idx_t xsq = x * x;
        idx_t rem = n - xsq;
        if (rem == 0)                 /* should not occur for non‑square n */
            continue;

        idx_t ymax = floor_sqrt_u64(rem);
        for (idx_t y = 1; y <= ymax; ++y)
        {
            idx_t ysq = y * y;
            if (rem % ysq == 0)
                ++count;
        }
    }

    return count;
}
