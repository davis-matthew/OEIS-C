/*
 * A183043  Triangular array, T(i,j)=number of knight's moves to points on
 *          vertical segments (n,0), (n,1),...,(n,n) on infinite chessboard.
 * https://oeis.org/A183043
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = knight_distance(i, j) where i = floor((sqrt(8*n + 1) - 1)
 *            / 2) j = n - i*(i + 1)/2 knight_distance(x, y) is the minimal
 *            number of knight moves from (0,0) to (x,y) on an infinite
 *            chessboard
 * Range:     u8 n <= 10010, u16 n <= 10010, u32 n <= 10010, u64 n <= 10010,
 *            u128 n <= 10010, big any n
 * Validated: standard, pass, 107 terms through n = 9927 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <math.h>

static inline seq_t knight_distance(seq_t x, seq_t y)
{
    /* ensure x >= y */
    if (x < y) {
        seq_t t = x;
        x = y;
        y = t;
    }

    /* special cases */
    if (x == (seq_t)1 && y == (seq_t)0) return (seq_t)3;
    if (x == (seq_t)2 && y == (seq_t)2) return (seq_t)4;

    /* ceil division helpers */
    seq_t a = (x + (seq_t)1) / (seq_t)2; /* ceil(x/2) */
    seq_t b = (y + (seq_t)1) / (seq_t)2; /* ceil(y/2) */
    seq_t c = (x + y + (seq_t)2) / (seq_t)3; /* ceil((x+y)/3) */

    seq_t d = a;
    if (b > d) d = b;
    if (c > d) d = c;

    /* adjust parity */
    if ( (d & (seq_t)1) != ((x + y) & (seq_t)1) )
        d += (seq_t)1;

    return d;
}

seq_t a(idx_t n)
{
    /* compute row index i */
    double root = sqrt((double)8.0 * (double)n + 1.0);
    idx_t i = (idx_t)((root - 1.0) / 2.0);

    /* correct possible rounding errors */
    while ( (i * (i + 1)) / 2 > n ) i--;
    while ( ((i + 1) * (i + 2)) / 2 <= n ) i++;

    /* column index j */
    idx_t j = n - (i * (i + 1)) / 2;

    return knight_distance((seq_t)i, (seq_t)j);
}
