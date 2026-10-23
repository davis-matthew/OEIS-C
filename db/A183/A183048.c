/*
 * A183048  Sums of least number of knight's moves on boundaries of squares
 *          [-n,n]x[-n,n] on infinite chessboard.
 * https://oeis.org/A183048
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sum of the minimal number of knight moves from (0,0) to
 *            each square on the boundary of the square [-n,n]×[-n,n] on an
 *            infinite chessboard. The distance function is computed with a
 *            known closed‑form formula for the knight on an infinite board;
 *            the implementation is O(n) for each call
 * Range:     u8 n <= 6, u16 n <= 38, u32 n <= 38, u64 n <= 38, u128 n <= 38,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Helper: minimal knight distance from (0,0) to (x,y), with x>=0, y>=0. */
static seq_t knight_dist(idx_t x, idx_t y)
{
    /* Ensure x >= y */
    if (x < y) {
        idx_t t = x; x = y; y = t;
    }
    /* Special cases */
    if (x == 1 && y == 0) return (seq_t)3;
    if (x == 2 && y == 2) return (seq_t)4;

    /* Generic formula */
    seq_t xx = (seq_t)x;
    seq_t yy = (seq_t)y;

    seq_t d1 = (xx + 1) / 2;
    seq_t d2 = (yy + 1) / 2;
    seq_t d3 = (xx + yy + 2) / 3;

    seq_t d = d1 > d2 ? d1 : d2;
    if (d3 > d) d = d3;

    /* Parity correction */
    if ((d % 2) != ((xx + yy) % 2)) d++;

    return d;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;

    seq_t sum = 0;
    idx_t nval = n;

    /* Corners: (±n,±n) – 4 points */
    seq_t d_corner = knight_dist(nval, nval);
    sum += (seq_t)4 * d_corner;

    /* Sides with x = ±n,  y from -n+1 .. n-1 */
    for (idx_t y = 0; y <= nval - 1; ++y) {
        seq_t d = knight_dist(nval, y);
        if (y == 0)
            sum += (seq_t)2 * d;      /* (±n,0) – two points */
        else
            sum += (seq_t)4 * d;      /* (±n,±y) – four points */
    }

    /* Sides with y = ±n,  x from -n+1 .. n-1 */
    for (idx_t x = 0; x <= nval - 1; ++x) {
        seq_t d = knight_dist(x, nval);
        if (x == 0)
            sum += (seq_t)2 * d;      /* (0,±n) – two points */
        else
            sum += (seq_t)4 * d;      /* (±x,±n) – four points */
    }

    return sum;
}
