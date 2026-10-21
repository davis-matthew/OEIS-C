/*
 * A071690  Number of ways to write n as n = x*y*z*t/(x+y+z+t) with 1 <= x <=
 *          y <= z <= t <= n.
 * https://oeis.org/A071690
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = number of quadruples (x,y,z,t) with 1 ≤ x ≤ y ≤ z ≤ t ≤ n
 *            and n = xyz*t/(x+y+z+t)
 * Range:     u8 n <= 17, u16 n <= 744, u32 n <= 9999, u64 n <= 9999,
 *            u128 n <= 9999, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <math.h>

seq_t a(idx_t nn)
{
    uint64_t n = (uint64_t)nn;          /* index as native unsigned */
    seq_t count = 0;                    /* result variable */

    /* x must satisfy x^3 ≤ 4n (from 4n ≥ xyz) and x ≤ n */
    for (uint64_t x = 1; x <= n && x*x*x <= 4*n; ++x)
    {
        /* bound for y: y^2 ≤ 4n/x  →  y ≤ sqrt(4n/x) */
        uint64_t y_limit = (uint64_t)sqrt((double)(4 * n) / (double)x);
        if (y_limit > n) y_limit = n;

        for (uint64_t y = x; y <= y_limit; ++y)
        {
            uint64_t xy = x * y;
            if (xy == 0) continue;

            /* z must satisfy n < xy*z ≤ 4n  and z ≤ n */
            uint64_t z_min = n / xy + 1;                 /* xyz > n   */
            uint64_t z_max = (4 * n) / xy;              /* xyz ≤ 4n  */
            if (z_max > n) z_max = n;
            if (z_min > z_max) continue;

            /* enforce ordering y ≤ z */
            uint64_t z_start = z_min;
            if (z_start < y) z_start = y;

            for (uint64_t z = z_start; z <= z_max; ++z)
            {
                seq_t xyz    = (seq_t)xy * (seq_t)z;      /* xyz as seq_t */
                seq_t denom  = xyz - (seq_t)n;            /* xyz - n   */
                if (denom == 0) continue;

                seq_t numer = (seq_t)n * ((seq_t)x + (seq_t)y + (seq_t)z);
                if (numer % denom != 0) continue;

                seq_t t = numer / denom;
                if (t < (seq_t)z || t > (seq_t)n) continue;

                ++count;
            }
        }
    }

    return count;
}
