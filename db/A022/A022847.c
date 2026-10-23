/*
 * A022847  Integer nearest n*sqrt(3).
 * https://oeis.org/A022847
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = nearest integer to n*sqrt(3)
 * Range:     u8 n <= 4, u16 n <= 65, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/floor_sqrt.h"

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;

    /* Compute 3*n*n */
    seq_t sq = nn * nn;             /* n^2   */
    sq = sq * ((seq_t)3);           /* 3*n^2 */

    /* y = floor(n*sqrt(3)) = floor_sqrt(3*n*n) */
    seq_t y  = floor_sqrt(sq);

    /* Determine whether to round up or down */
    seq_t y2  = y * y;
    seq_t nxt = y + 1;
    seq_t nxt2 = nxt * nxt;

    seq_t diff1 = sq - y2;          /* 3*n^2 - y^2   */
    seq_t diff2 = nxt2 - sq;        /* (y+1)^2 - 3*n^2 */

    if (diff2 < diff1)
        return nxt;                 /* round up */
    else
        return y;                   /* round down */
}
