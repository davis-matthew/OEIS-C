/*
 * A117609  Number of lattice points inside the ball x^2 + y^2 + z^2 <= n.
 * https://oeis.org/A117609
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of integer triples (x,y,z) with x^2 + y^2 + z^2 <=
 *            n
 * Range:     u8 n <= 15, u16 n <= 625, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Integer floor square root for a 64‑bit unsigned integer. */
static inline idx_t isqrt_u64(idx_t n)
{
    idx_t low = 0;
    idx_t high = n;
    idx_t ans = 0;
    while (low <= high)
    {
        idx_t mid = (low + high) >> 1;
        unsigned __int128 sq = (unsigned __int128)mid * mid;
        if (sq == n)
            return mid;
        if (sq < n)
        {
            ans = mid;
            low = mid + 1;
        }
        else
            high = mid - 1;
    }
    return ans;
}

/* Count lattice points (x,y) with x^2 + y^2 <= m. */
static inline seq_t count_2d(idx_t m)
{
    seq_t cnt = 0;
    idx_t xmax = isqrt_u64(m);

    /* x = 0 */
    idx_t ymax = isqrt_u64(m);
    cnt += (seq_t)(2 * ymax + 1);      /* y from -ymax .. ymax */

    /* x > 0 (symmetric +/-x) */
    for (idx_t x = 1; x <= xmax; ++x)
    {
        idx_t rem = m - x * x;
        idx_t y_max = isqrt_u64(rem);
        cnt += (seq_t)(2 * (2 * y_max + 1));  /* both x and -x */
    }

    return cnt;
}

seq_t a(idx_t n)
{
    seq_t total = 0;
    idx_t zmax = isqrt_u64(n);

    /* z = 0 */
    total += count_2d(n);

    /* z > 0, each contributes twice (z and -z) */
    for (idx_t z = 1; z <= zmax; ++z)
    {
        idx_t m = n - z * z;
        total += (seq_t)2 * count_2d(m);
    }

    return total;
}
