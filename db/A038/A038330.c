/*
 * A038330  Triangle whose (i,j)-th entry is binomial(i,j)*12^(i-j)*4^j.
 * https://oeis.org/A038330
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   a(n) = binomial(i,j)*12^(i-j)*4^j where i satisfies i*(i+1)/2 <=
 *            n < (i+1)*(i+2)/2 j = n - i*(i+1)/2 (column within row i)
 * Range:     u8 n <= 5, u16 n <= 14, u32 n <= 44, u64 n <= 154, u128 n <= 565,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    /* Find the row index i as the smallest integer with
       (i+1)*(i+2)/2 > n.  After the loop i is one larger than the
       desired row, so the row is i-1.                               */
    idx_t i = 0;
    while ((i * (i + 1)) / 2 <= n) {
        ++i;
    }
    idx_t row = i - 1;

    /* Column index j (row starts at zero).                      */
    idx_t prev = row * (row + 1) / 2;
    idx_t col  = n - prev;

    /* Base value from the triangle.                           */
    seq_t val = binomial(row, col);

    /* Multiply by 12^(row-col).                                */
    idx_t exp12 = row - col;
    seq_t pow12 = 1;
    for (idx_t k = 0; k < exp12; ++k) {
        pow12 *= (seq_t)12;
    }
    val *= pow12;

    /* Multiply by 4^col.                                       */
    idx_t exp4 = col;
    seq_t pow4 = 1;
    for (idx_t k = 0; k < exp4; ++k) {
        pow4 *= (seq_t)4;
    }
    val *= pow4;

    return val;
}
