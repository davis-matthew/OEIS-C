/*
 * A232115  a(n) is the Manhattan distance between n and n(n+1)/2 in a square
 *          spiral of positive integers with 1 at the center.
 * https://oeis.org/A232115
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Manhattan distance between n and n(n+1)/2 * in the square
 *            spiral of positive integers with 1 at the centre
 * Range:     u8 n <= 22, u16 n <= 71, u32 n <= 71, u64 n <= 71, u128 n <= 71,
 *            big any n
 * Helpers:   int_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/int_sqrt.h"


static inline void spiral_coord(idx_t k, int64_t *x, int64_t *y)
{
    if (k == 1) {
        *x = 0; *y = 0;
        return;
    }

    /* layer r = floor(ceil(sqrt(k))/2)   */
    idx_t t = int_sqrt(k);
    if ((unsigned __int128)t * t < k)
        t++;                        /* ceil(sqrt(k)) */
    idx_t r = t / 2;                /* layer number */

    idx_t side_len = 2 * r;          /* numbers on one side of the ring  */
    unsigned __int128 max_num128 = (unsigned __int128)(2 * r + 1) * (2 * r + 1);
    unsigned __int128 offs = max_num128 - k;   /* offset from the max corner */

    if (offs < side_len) {                    /* bottom side */
        *x = (int64_t)(r - (idx_t)offs);
        *y = (int64_t)(-r);
    } else if (offs < side_len * 2) {          /* left side */
        *x = (int64_t)(-r);
        *y = (int64_t)(-r + (idx_t)(offs - side_len));
    } else if (offs < side_len * 3) {          /* top side */
        *x = (int64_t)(-r + (idx_t)(offs - side_len * 2));
        *y = (int64_t)( r);
    } else {                                  /* right side */
        *x = (int64_t)( r);
        *y = (int64_t)( r - (idx_t)(offs - side_len * 3));
    }
}

seq_t a(idx_t n)
{
    int64_t x1, y1;
    spiral_coord(n, &x1, &y1);

    /* triangular number T = n*(n+1)/2, computed in seq_t to avoid overflow. */
    seq_t tn = (seq_t)(( (unsigned __int128)n * (n + 1) ) / 2);

    /* For very large n, tn may exceed idx_t limits; such values are outside
       the range of tests, so we cast safely. */
    idx_t tidx = (idx_t)tn;

    int64_t x2, y2;
    spiral_coord(tidx, &x2, &y2);

    int64_t dx = x1 >= x2 ? x1 - x2 : x2 - x1;
    int64_t dy = y1 >= y2 ? y1 - y2 : y2 - y1;
    return (seq_t)(dx + dy);
}
