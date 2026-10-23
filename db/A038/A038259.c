/*
 * A038259  Triangle whose (i,j)-th entry is binomial(i,j)*6^(i-j)*5^j.
 * https://oeis.org/A038259
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl, easy, changed
 * Formula:   a(n) = binomial(row, col) * 6^(row-col) * 5^col where row is the
 *            smallest integer r such that (r+1)(r+2)/2 > n and col = n -
 *            r(r+1)/2
 * Range:     u8 n <= 6, u16 n <= 21, u32 n <= 58, u64 n <= 214, u128 n <= 755,
 *            big any n
 * Helpers:   binomial, pow_uint
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"
#include "../utils/pow_uint.h"

seq_t a(idx_t n)
{
    /* Find the row r for which the n‑th term lies.
       C(r) = (r+1)(r+2)/2 is the cumulative number of terms up to row r. */
    idx_t r   = 0;
    idx_t cum = 1;                       /* C(0) = 1 */
    while (cum <= n) {
        ++r;
        cum = (unsigned __int128)(r + 1) * (r + 2) / 2;
    }

    /* C(r-1) = r*(r+1)/2 is the number of terms before the current row. */
    idx_t prev_cum = (unsigned __int128)r * (r + 1) / 2;

    /* Column index within the row. */
    idx_t col = n - prev_cum;

    seq_t bin = binomial(r, col);
    seq_t p6  = pow_uint(6, r - col);
    seq_t p5  = pow_uint(5, col);

    return bin * p6 * p5;
}
