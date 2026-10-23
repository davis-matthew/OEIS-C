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
 * Range:     u8 n <= 9927, u16 n <= 9927, u32 n <= 9927, u64 n <= 9927,
 *            u128 n <= 9927, big any n
 * Helpers:   knight_distance
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <math.h>
#include "../utils/knight_distance.h"

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
