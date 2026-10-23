/*
 * A126937  A dragon curve is drawn on an Ulam spiral; a(n) is the integer
 *          written in the cell reached at step n.
 * https://oeis.org/A126937
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the Ulam spiral index of the point that the * Heighway
 *            dragon curve reaches after n steps, starting at the * origin
 *            (0,0) heading east (positive x). The turn after step t * (t =
 *            1,2,…) is a right turn if the bit at position tz+1 of t * (where
 *            tz is the number of trailing zeros in t) is 0,...
 * Range:     u8 n <= 61, u16 n <= 64, u32 n <= 64, u64 n <= 64, u128 n <= 64,
 *            big any n
 * Helpers:   ulam_index
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/ulam_index.h"

seq_t a(idx_t n)
{
    if (n == 0) {
        return (seq_t)0;
    }

    int64_t posx = 0;
    int64_t posy = 0;
    int64_t dx   = 1;   /* initial direction: east (positive x) */
    int64_t dy   = 0;

    for (idx_t step = 0; step < n; ++step) {
        /* move one step */
        posx += dx;
        posy += dy;

        if (step + (idx_t)1 == n) {
            break;   /* reached requested step */
        }

        /* determine turn for next direction */
        idx_t t = step + (idx_t)1;           /* t is 1‑based turn number */
        /* count trailing zeros of t */
        idx_t tz = (idx_t)__builtin_ctzll((unsigned long long)t);
        /* right turn if bit at position tz+1 of t is 0 */
        bool rightTurn = (((t >> (tz + (idx_t)1)) & (idx_t)1ULL) == (idx_t)0ULL);

        if (rightTurn) {
            /* clockwise 90° rotation: (dx,dy) -> (dy,-dx) */
            int64_t tmp = dx;
            dx = dy;
            dy = -tmp;
        } else {
            /* counter‑clockwise 90° rotation: (dx,dy) -> (-dy,dx) */
            int64_t tmp = dx;
            dx = -dy;
            dy = tmp;
        }
    }

    return ulam_index(posx, posy);
}
