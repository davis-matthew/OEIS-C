/*
 * A115951  Expansion of 1/sqrt(1-4*x*y-4*x^2*y).
 * https://oeis.org/A115951
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn, tabl
 * Formula:   a(n) is obtained by expanding the bivariate generating function
 *            1/sqrt(1-4*x*y-4*x^2*y). The coefficient sequence is produced by
 *            flattening the triangle T(r,c)=C(2c,c)*C(c,r-c) row by row (rows
 *            start at r=0, each row has r+1 entries). Thus for a given
 *            overall index n we find the unique row r and...
 * Range:     u8 n <= 13, u16 n <= 43, u32 n <= 165, u64 n <= 520,
 *            u128 n <= 1877, big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binom.h"

seq_t a(idx_t n)
{
    /* Find the row r such that cum + (r+1) > n.  The variable cum
       holds the total number of elements before the current row. */
    idx_t r   = 0;
    idx_t cum = 0;
    while (cum + (r + 1) <= n) {
        cum += r + 1;
        ++r;
    }

    /* Column index within the found row */
    idx_t c = n - cum;

    /* Compute the triangle entry: binom(2c,c) * binom(c,r-c) */
    seq_t first  = binom(2 * c, c);
    seq_t second = binom(c, r - c);
    return first * second;
}
