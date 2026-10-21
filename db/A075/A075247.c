/*
 * A075247  Largest possible z-value of an integer solution (x,y,z) to 4/n =
 *          1/x + 1/y + 1/z satisfying 0 < x < y < z. The x and y components
 *          are in A075245 and A075246.
 * https://oeis.org/A075247
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  hard, nice, nonn
 * Formula:   a(n) = largest z such that 4/n = 1/x + 1/y + 1/z with 0 < x < y
 *            < z. Algorithm: for each candidate x > n/4 compute the smallest
 *            y that gives a positive denominator d = 4xy – n(x+y). The
 *            largest z for that x is obtained when d is minimal; we search y
 *            in increasing order until we find a y where d d...
 * Range:     u8 n <= 8, u16 n <= 34, u32 n <= 514, u64 n <= 2000,
 *            u128 n <= 2000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

seq_t a(idx_t n)
{
    if (n < 3)
        return 0;                  /* undefined for offset < 3 */

    seq_t best_z = 0;

    /* minimal x such that 4x > n */
    idx_t x_start = n / 4 + 1;
    if (x_start < 1)
        x_start = 1;

    /* safe upper bound for x */
    idx_t x_max = n * 2;

    for (idx_t x = x_start; x <= x_max; ++x)
    {
        idx_t denom_factor = 4ULL * x - n;     /* > 0 by construction */
        if (denom_factor == 0)
            continue;

        /* smallest y that makes the denominator positive */
        idx_t y_start = (n * x) / denom_factor + 1;
        if (y_start <= x)
            y_start = x + 1;

        /* theoretical upper bound for z when y = y_start */
        seq_t num_upper   = (seq_t)n * x * y_start;
        seq_t denom_upper = (seq_t)denom_factor * y_start - (seq_t)n * x;
        if (denom_upper == 0)
            continue;
        seq_t z_upper = num_upper / denom_upper;
        if (z_upper <= best_z)
            break;   /* further x will only give smaller upper bounds */

        /* search for the smallest y >= y_start that makes the expression integral */
        for (idx_t y = y_start; ; ++y)
        {
            seq_t d   = (seq_t)4ULL * x * y - (seq_t)n * (x + y);
            seq_t num = (seq_t)n * x * y;

            if (num % d == 0)          /* integral solution found */
            {
                seq_t z = num / d;
                if (z > best_z)
                    best_z = z;
                break;                 /* larger y will only decrease z */
            }

            /* If even the real (not necessarily integral) value of z is
               <= best_z, no larger z can be obtained with this (x,y). */
            seq_t z_real = num / d;
            if (z_real <= best_z)
                break;
        }
    }

    return best_z;
}
