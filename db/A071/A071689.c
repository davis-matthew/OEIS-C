/*
 * A071689  Number of ways to write n as n = x*y*z*t+x+y+z+t 0<=x<=y<=z<=t<=n.
 * https://oeis.org/A071689
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = #{(x,y,z,t) : 0 ≤ x ≤ y ≤ z ≤ t ≤ n, n = x*y*z*t + x + y
 *            + z + t}
 * Range:     u8 n <= 52, u16 n <= 199, u32 n <= 199, u64 n <= 199,
 *            u128 n <= 199, big any n
 * Helpers:   prod_gt_limit
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/prod_gt_limit.h"

seq_t a(idx_t n)
{
    seq_t count = 0;

    for (idx_t x = 0; x <= n; ++x)
    {
        for (idx_t y = x; y <= n; ++y)
        {
            /* stop if x*y*y > n */
            if (prod_gt_limit(x, y, y, n)) break;

            for (idx_t z = y; z <= n; ++z)
            {
                /* stop if x*y*z > n */
                if (prod_gt_limit(x, y, z, n)) break;

                /* sum of x, y, z */
                unsigned __int128 sum_xyz128 = (unsigned __int128)x + y + z;
                if (sum_xyz128 > n) break;        /* larger z only increases the sum */
                idx_t sum_xyz = (idx_t)sum_xyz128;

                idx_t numerator = n - sum_xyz;    /* n - (x + y + z) ≥ 0 */

                unsigned __int128 prod_xyz128 = (unsigned __int128)x * y * z; /* ≤ n, fits in 128-bit */
                idx_t denom = (idx_t)(prod_xyz128 + 1); /* 1 + x*y*z ≤ n+1 */

                if (numerator % denom) continue;   /* t must be integer */

                idx_t t = numerator / denom;
                if (t < z) continue;              /* ordering constraint */

                ++count;                           /* found a valid quadruple */
            }
        }
    }

    return count;
}
