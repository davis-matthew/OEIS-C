/*
 * A189972  Number of 3:4:5 proportioned triangles on a (n+1)X(n+1) grid.
 * https://oeis.org/A189972
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of 3:4:5 proportioned right triangles on an
 *            (n+1)x(n+1) grid
 * Range:     u8 n <= 7, u16 n <= 30, u32 n <= 99, u64 n <= 99, u128 n <= 99,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* The count will be accumulated in a seq_t variable. */
    seq_t result = 0;

    /* Convert the grid size to a signed integer for arithmetic. */
    int N = (int)n;

    /* The integer parameters a and b satisfy |a|, |b| <= floor(n/4). */
    int limit = N / 4;

    for (int a = -limit; a <= limit; ++a)
    {
        for (int b = -limit; b <= limit; ++b)
        {
            if (a == 0 && b == 0) continue;   /* skip the zero vector */

            /* The longer leg (length 4k) is represented by u = (4a, 4b). */
            int dx_u = 4 * a;
            int dy_u = 4 * b;

            /* First orientation: v = (-3b, 3a). */
            int dx_v = -3 * b;
            int dy_v = 3 * a;

            /* Compute the allowed range for the right‑angle vertex coordinates. */
            int lx = 0;
            if (-dx_u > lx) lx = -dx_u;
            if (-dx_v > lx) lx = -dx_v;
            int ux = N;
            if (N - dx_u < ux) ux = N - dx_u;
            if (N - dx_v < ux) ux = N - dx_v;
            if (lx <= ux)
            {
                int ly = 0;
                if (-dy_u > ly) ly = -dy_u;
                if (-dy_v > ly) ly = -dy_v;
                int uy = N;
                if (N - dy_u < uy) uy = N - dy_u;
                if (N - dy_v < uy) uy = N - dy_v;
                if (ly <= uy)
                {
                    int countX = ux - lx + 1;
                    int countY = uy - ly + 1;
                    result += (seq_t)countX * (seq_t)countY;
                }
            }

            /* Second orientation: v = (3b, -3a). */
            dx_v = 3 * b;
            dy_v = -3 * a;

            lx = 0;
            if (-dx_u > lx) lx = -dx_u;
            if (-dx_v > lx) lx = -dx_v;
            ux = N;
            if (N - dx_u < ux) ux = N - dx_u;
            if (N - dx_v < ux) ux = N - dx_v;
            if (lx <= ux)
            {
                int ly = 0;
                if (-dy_u > ly) ly = -dy_u;
                if (-dy_v > ly) ly = -dy_v;
                int uy = N;
                if (N - dy_u < uy) uy = N - dy_u;
                if (N - dy_v < uy) uy = N - dy_v;
                if (ly <= uy)
                {
                    int countX = ux - lx + 1;
                    int countY = uy - ly + 1;
                    result += (seq_t)countX * (seq_t)countY;
                }
            }
        }
    }

    return result;
}
