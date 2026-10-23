/*
 * A110509  Riordan array (1, x(1-2x)).
 * https://oeis.org/A110509
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, sign, tabl
 * Formula:   a(n) = T(row, col) of the Riordan array (1, x(1-2x)), where *
 *            row is the unique integer such that row*(row+1)/2 <= n <
 *            (row+1)*(row+2)/2 * col = n - row*(row+1)/2 * T(row, col) =
 *            (-2)^(row-col) * binomial(col, row-col)
 * Range:     i8 n <= 50, i16 n <= 181, i32 n <= 616, i64 n <= 1274,
 *            i128 n <= 1274, big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    /* Find the row r such that r(r+1)/2 <= n < (r+1)(r+2)/2 */
    idx_t r = 0;
    while ( (r+1)*(r+2)/2 <= n )
        ++r;
    /* Column within the row */
    idx_t start = r*(r+1)/2;
    idx_t c = n - start;

    /* m = row - col = r - c */
    idx_t m = r - c;

    /* Compute binomial(c, m) */
    seq_t bin = binomial(c, m);

    /* Compute 2^m */
    seq_t pow2 = 1;
    for (idx_t i = 0; i < m; ++i)
        pow2 *= 2;

    /* Apply sign (-1)^m */
    if (m & 1)
        pow2 = -pow2;

    /* Result */
    return bin * pow2;
}
