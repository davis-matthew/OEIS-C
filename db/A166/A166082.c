/*
 * A166082  Maximal volume of a closed box created by using at most n voxels
 *          as the boundary.
 * https://oeis.org/A166082
 *
 * Offset:    8    (a(8) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = max_{w,h,d >= 2} { w*h*d | w*h*d - (w-2)*(h-2)*(d-2) <= n
 *            }
 * Range:     u8 n <= 78, u16 n <= 78, u32 n <= 78, u64 n <= 78, u128 n <= 78,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The algorithm enumerates all possible (w,h) pairs, computes the maximal feasible d
   using the inequality, and keeps the largest volume found. */

#include <stdint.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    if (n < 8)
        return 0;          /* undefined for n < 8 */

    seq_t best = 0;

    /* Maximum width w is bounded by the condition 4*w <= n (minimal boundary with
       h = d = 2). */
    idx_t w_max = n / 4;

    for (idx_t w = 2; w <= w_max; ++w)
    {
        /* For fixed w, the boundary with h = 2 and d = 2 is 2*w*h.
           Hence h cannot exceed n/(2*w). */
        idx_t h_max = n / (2 * w);
        if (h_max < 2)
            continue;       /* no feasible h for this w */

        for (idx_t h = 2; h <= h_max; ++h)
        {
            /* Numerator and denominator in the derived bound for d:
               d <= (n - 2*w*h + 4*w + 4*h - 8) / (2*w + 2*h - 4)             */
            seq_t numer = (seq_t)n
                          - 2 * (seq_t)w * (seq_t)h
                          + 4 * (seq_t)w
                          + 4 * (seq_t)h
                          - 8;

            seq_t denom = 2 * (seq_t)w + 2 * (seq_t)h - 4;

            /* If the bound is less than 2, no admissible d exists for this (w,h). */
            if (numer < denom * 2)
                continue;

            seq_t d_max = numer / denom;   /* floor division gives maximal d */

            /* Compute volume and update best. */
            seq_t vol = (seq_t)w * (seq_t)h * d_max;
            if (vol > best)
                best = vol;
        }
    }

    return best;
}
