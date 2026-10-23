/*
 * A183049  Array of least knight's moves to points (n,0), (n-1,1), ...,
 *          (1,n-1) on infinite chessboard.
 * https://oeis.org/A183049
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   compute a(n) as the minimal number of knight moves from (0,0) to
 *            the point (x,y) where n enumerates the points
 *            (n,0),(n-1,1),...,(1,n-1) as described in A183049. The distance
 *            is obtained by the closed‐form expression for an infinite
 *            chessboard (see A065775)
 * Range:     u8 n <= 87, u16 n <= 87, u32 n <= 87, u64 n <= 87, u128 n <= 87,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

/* Helper: minimal knight moves from (0,0) to (x,y), 0 <= y <= x */
static seq_t knight_moves(seq_t x, seq_t y)
{
    if ((x == 0) && (y == 0))
        return 0;
    if ((x == 1) && (y == 0))
        return 3;
    if ((x == 0) && (y == 1))
        return 3;
    if ((x == 2) && (y == 2))
        return 4;

    const seq_t one  = 1;
    const seq_t two  = 2;
    const seq_t three = 3;

    seq_t d1 = (x + one) / two;          /* ceil(x/2) */
    seq_t d2 = (y + one) / two;          /* ceil(y/2) */
    seq_t d3 = (x + y + two) / three;    /* ceil((x+y)/3) */

    seq_t d = d1;
    if (d2 > d) d = d2;
    if (d3 > d) d = d3;

    /* Adjust parity: knight moves alternate parity each step */
    if ((d & one) != ((x + y) & one))
        d += one;

    return d;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n == 1)          /* the origin (0,0) */
        return 0;

    /* Remove the first element (the origin) from the counting. */
    idx_t m = n - 2;  /* zero‑based index among the points after the origin */

    /* Find the row r such that r*(r+1)/2 > m. */
    idx_t r = 1;
    while (( (__uint128_t)r * (r + 1) ) / 2 <= m)
        ++r;

    /* Position within the row (0 ≤ pos < r) */
    idx_t pos = m - ( ( (__uint128_t)(r - 1) * r ) / 2 );

    /* Coordinates of the point: (x, y) = (r - pos, pos) */
    seq_t x = (seq_t)r - (seq_t)pos;
    seq_t y = (seq_t)pos;

    return knight_moves(x, y);
}
