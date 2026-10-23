/*
 * A028590  Expansion of (theta_3(z)*theta_3(6z)+theta_2(z)*theta_2(6z)).
 * https://oeis.org/A028590
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = #{ (x,y)∈ℤ² | x² + 6y² = n and x ≡ y (mod 2) }
 * Range:     u8 n <= 63, u16 n <= 80, u32 n <= 80, u64 n <= 80, u128 n <= 80,
 *            big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/floor_sqrt.h"

seq_t a(idx_t n)
{
    seq_t count = 0;
    seq_t nval  = (seq_t)n;

    for (idx_t y = 0;; ++y)
    {
        seq_t yterm = (seq_t)6 * (seq_t)y * (seq_t)y;
        if (yterm > nval) break;

        seq_t rem = nval - yterm;
        seq_t xroot = floor_sqrt(rem);
        if (xroot * xroot != rem) continue;

        /* Parity condition: x ≡ y (mod 2) */
        if ((xroot & (seq_t)1) != ( (seq_t)y & (seq_t)1 )) continue;

        seq_t mult = ((xroot == 0) ? 1 : 2) * ((y == 0) ? 1 : 2);
        count += mult;
    }

    return count;
}
