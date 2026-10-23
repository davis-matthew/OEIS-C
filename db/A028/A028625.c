/*
 * A028625  Expansion of (theta_3(z)*theta_3(15z)+theta_2(z)*theta_2(15z)).
 * https://oeis.org/A028625
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of integer solutions (x,y) to x² + 15*y² = 4*n
 * Range:     u8 n <= 15, u16 n <= 4020, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/floor_sqrt.h"

seq_t a(idx_t n)
{
    seq_t count = 0;
    seq_t four_n = (seq_t)4 * (seq_t)n;

    for (idx_t y = 0; ; ++y)
    {
        seq_t y_sq   = (seq_t)y * (seq_t)y;
        seq_t prod   = (seq_t)15 * y_sq;
        if (prod > four_n) break;

        seq_t t    = four_n - prod;
        seq_t s    = floor_sqrt(t);
        if (s * s == t)
        {
            seq_t ways = (s == 0) ? (seq_t)1 : (seq_t)2;
            if (y == 0)
                count += ways;
            else
                count += ways * (seq_t)2;   /* ±y */
        }
    }
    return count;
}
