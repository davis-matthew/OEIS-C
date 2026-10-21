/*
 * A217292  Permutation of natural numbers arising from applying the walk of
 *          right triangular type-2 spiral (defined in A214251) to the data of
 *          square spiral (e.g. A214526).
 * https://oeis.org/A217292
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 67, u16 n <= 67, u32 n <= 67, u64 n <= 67, u128 n <= 67,
 *            big any n
 * Helpers:   square_spiral_value
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation: a(n) is the value in a square spiral at the n‑th
 * point visited by the right triangular type‑2 spiral walk
 * (A214251 applied to the data of the square spiral A214526). */

#include <stdint.h>
#include "../utils/square_spiral_value.h"

seq_t a(idx_t n)
{
    if (n == 1)
        return (seq_t)1;   /* (0,0) */

    int64_t x = 0, y = 0;        /* current position */
    idx_t remaining = n - 1;     /* steps to move after the start */

    for (idx_t seg = 1; remaining > 0; ++seg) {
        idx_t steps = seg;
        if (steps > remaining)
            steps = remaining;

        int dx, dy;
        /* segment direction: 1 -> down, 2 -> right, 0 -> diag (-1,1) */
        switch (seg % 3) {
            case 1: dx = 0; dy = -1; break;   /* down  */
            case 2: dx = 1; dy = 0;  break;   /* right */
            default: dx = -1; dy = 1; break;  /* diagonal */
        }

        x += (int64_t)dx * (int64_t)steps;
        y += (int64_t)dy * (int64_t)steps;

        remaining -= steps;
    }

    return square_spiral_value(x, y);
}
