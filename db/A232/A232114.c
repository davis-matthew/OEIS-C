/*
 * A232114  a(n) is the Manhattan distance between n and n^2 in a square
 *          spiral of positive integers with 1 at the center.
 * https://oeis.org/A232114
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 70, u32 n <= 70, u64 n <= 70, u128 n <= 70,
 *            big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A232114: Manhattan distance between n and n^2 in the square spiral
   of positive integers with 1 at the center. */

/* the helper int floor_sqrt(seq_t x) is provided in the build
   environment; include its header. */
#include <stdint.h>
#include "../utils/floor_sqrt.h"

static inline void spiral_coord(seq_t k, int64_t *x, int64_t *y)
{
    /* k is assumed ≥ 1; the caller passes 1…n or n². */
    seq_t s   = floor_sqrt(k);           /* floor(√k) */
    seq_t ssq = s * s;
    seq_t ceil_s = (ssq == k) ? s : s + 1;   /* ceil(√k) */

    /* ring index r = ceil(√k) / 2  (integer division) */
    int64_t r = (int64_t)(ceil_s / 2);

    if (r == 0) {                         /* the centre */
        *x = *y = 0;
        return;
    }

    /* side length of the ring (number of cells on one side) = 2r+1 */
    seq_t r_s = (seq_t)r;
    seq_t p   = 2 * r_s + 1;              /* side length 2r+1 */
    seq_t max = p * p;                     /* largest number in ring r */
    seq_t t   = max - k;                   /* offset from max, 0 … 8r-1 */

    seq_t side = t / (seq_t)(2 * r);       /* which side: 0–3 */
    seq_t rem  = t % (seq_t)(2 * r);       /* offset along that side */

    int64_t side_i = (int64_t)side;
    int64_t rem_i  = (int64_t)rem;

    /* walk around the ring, starting at (r, -r) and going counter‑clockwise
       through: bottom, left, top, right. */
    switch (side_i) {
        case 0: /* bottom side, moving left */
            *x = r - rem_i;
            *y = -r;
            break;
        case 1: /* left side, moving up */
            *x = -r;
            *y = -r + rem_i;
            break;
        case 2: /* top side, moving right */
            *x = -r + rem_i;
            *y = r;
            break;
        case 3: /* right side, moving down */
            *x = r;
            *y = r - rem_i;
            break;
        default: /* should never happen */
            *x = *y = 0;
            break;
    }
}

seq_t a(idx_t n)
{
    int64_t x_n, y_n, x_q, y_q;

    spiral_coord((seq_t)n, &x_n, &y_n);           /* coordinates of n */
    spiral_coord((seq_t)n * (seq_t)n, &x_q, &y_q);/* coordinates of n² */

    seq_t dx = (x_n > x_q) ? (seq_t)(x_n - x_q) : (seq_t)(x_q - x_n);
    seq_t dy = (y_n > y_q) ? (seq_t)(y_n - y_q) : (seq_t)(y_q - y_n);

    return dx + dy;
}
