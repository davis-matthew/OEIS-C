/*
 * A177722  Number of line segments connecting exactly 6 points in an n x n
 *          grid of points.
 * https://oeis.org/A177722
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) counts unordered pairs of grid points (i1,j1),(i2,j2) in an
 *            n×n grid such that the greatest common divisor of |i1−i2| and
 *            |j1−j2| is 5. Equivalently, line segments with exactly 6 lattice
 *            points (gcd(dx,dy)+1=6). The computation uses the identity: a(n)
 *            = 2·n·(n−5) // horizontal + vertical seg...
 * Range:     u8 n <= 11, u16 n <= 44, u32 n <= 44, u64 n <= 44, u128 n <= 44,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

static inline idx_t gcd_idx(idx_t a, idx_t b)
{
    while (b)
    {
        idx_t r = a % b;
        a = b;
        b = r;
    }
    return a;
}

seq_t a(idx_t n)
{
    if (n < 5)
        return 0;

    seq_t N = (seq_t)n;
    seq_t ans = 2 * N * (N - 5);          /* horizontal + vertical segments */

    idx_t maxU = (n - 1) / 5;              /* upper bound for 5*u and 5*v */

    for (idx_t u = 1; u <= maxU; ++u)
    {
        seq_t du = N - (seq_t)5 * u;
        if (du == 0) continue;            /* safety: when n is very small */
        for (idx_t v = 1; v <= maxU; ++v)
        {
            if (gcd_idx(u, v) == 1)
            {
                seq_t dv = N - (seq_t)5 * v;
                ans += 2 * du * dv;        /* count both dy>0 and dy<0 */
            }
        }
    }
    return ans;
}
