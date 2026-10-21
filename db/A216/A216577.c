/*
 * A216577  Number of nonnegative integer solutions to the equation x^2 +
 *          10*y^2 = n.
 * https://oeis.org/A216577
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of nonnegative integer solutions to x^2 + 10*y^2 =
 *            n
 * Range:     u8 n <= 99, u16 n <= 99, u32 n <= 99, u64 n <= 99, u128 n <= 99,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

/* Integer square root for idx_t (unsigned 64‑bit).  Uses binary search and
   __int128 to avoid overflow during multiplication. */
static inline idx_t isqrt_u64(idx_t v)
{
    idx_t low = 0, high = v;
    while (low <= high)
    {
        idx_t mid = low + ((high - low) >> 1);
        unsigned __int128 sq = (unsigned __int128)mid * mid;
        if (sq == v) return mid;
        if (sq < v)
            low = mid + 1;
        else
        {
            if (mid == 0) break;
            high = mid - 1;
        }
    }
    return low - 1;
}

/* Test whether a idx_t value is a perfect square. */
static inline bool is_square_idx(idx_t v)
{
    idx_t s = isqrt_u64(v);
    return (unsigned __int128)s * s == v;
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    seq_t count = 0;
    /* Max y such that 10*y^2 <= n. */
    idx_t max_y = isqrt_u64(n / 10U);
    for (idx_t y = 0; y <= max_y; ++y)
    {
        idx_t rem = n - 10U * y * y;
        if (is_square_idx(rem))
            ++count;
    }
    return count;
}
