/*
 * A224447  Multiplicities for representations of n as primitive and distinct
 *          sums of three squares.
 * https://oeis.org/A224447
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of primitive distinct representations of n as a^2
 *            + b^2 + c^2 with 0 <= a < b < c and gcd(a,b,c) = 1. Computed by
 *            direct search
 * Range:     u8 n <= 64, u16 n <= 130, u32 n <= 130, u64 n <= 130,
 *            u128 n <= 130, big any n
 * Helpers:   floor_sqrt, gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/gcd.h"
#include "../utils/floor_sqrt.h"

seq_t a(idx_t n)
{
    seq_t n_sq = (seq_t)n;     /* avoid repeated casts to seq_t */
    seq_t count = 0;

    /* iterate over possible a, b, c respecting a < b < c and a^2 + b^2 + c^2 = n   */
    for (idx_t a_val = 0; (seq_t)a_val * a_val <= n_sq; ++a_val)
    {
        seq_t a2 = (seq_t)a_val * a_val;
        seq_t remaining_after_a = n_sq - a2;

        for (idx_t b_val = a_val + 1;
             (seq_t)b_val * b_val <= remaining_after_a; ++b_val)
        {
            seq_t b2 = (seq_t)b_val * b_val;
            seq_t rem = remaining_after_a - b2;
            if (rem == 0) continue;            /* c would be 0 but need c > b */

            seq_t c_val = floor_sqrt(rem);
            if (c_val * c_val != rem) continue; /* not a perfect square */
            if (c_val <= b_val) continue;       /* enforce a < b < c */

            /* check primitivity: gcd(a,b,c) == 1 */
            seq_t g = gcd((seq_t)a_val, (seq_t)b_val);
            g = gcd(g, (seq_t)c_val);
            if (g == 1) ++count;
        }
    }

    return count;
}
