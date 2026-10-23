/*
 * A177724  Number of line segments connecting exactly 8 points in an n x n
 *          grid of points.
 * https://oeis.org/A177724
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of unordered point pairs in an n×n grid * whose
 *            difference vector (dx,dy) satisfies gcd(|dx|,|dy|)=7. * This
 *            counts line segments containing exactly 8 lattice points
 * Range:     u8 n <= 13, u16 n <= 46, u32 n <= 46, u64 n <= 46, u128 n <= 46,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    if (n < 8) return 0;

    seq_t result = 0;
    idx_t maxh = (n - 1) / 7;          /* maximum multiplier of 7 */

    /* Count all directed vectors (dx,dy) with dx > 0 or dx == 0 & dy > 0,
       except the vertical case handled separately. */
    for (idx_t h = 1; h <= maxh; ++h) {
        seq_t dx = 7 * (seq_t)h;

        for (int k = -(int)maxh; k <= (int)maxh; ++k) {
            /* Skip the zero vector and horizontal segments longer than 7.
               Only (dx=7,dy=0) is allowed. */
            if (k == 0 && h != 1) continue;

            /* Skip non‑coprime pairs: gcd(|dx|,|dy|) must be 7, i.e.
               gcd(h,|k|) must be 1. */
            if (gcd((seq_t)h, (seq_t)(k < 0 ? -k : k)) != 1) continue;

            /* |dy| in grid units */
            seq_t dy_abs = 7 * (seq_t)(k < 0 ? -k : k);

            /* Number of starting positions for this direction */
            seq_t mx = (seq_t)n - dx;
            seq_t my = (seq_t)n - dy_abs;
            if (mx == 0 || my == 0) continue;   /* no room for the segment */

            result += mx * my;
        }
    }

    /* Vertical segments (dx = 0, dy = ±7).  Only one orientation is needed
       for unordered pairs. */
    seq_t mx = (seq_t)n;
    seq_t my = (seq_t)n - 7;
    if (my > 0) result += mx * my;

    return result;
}
