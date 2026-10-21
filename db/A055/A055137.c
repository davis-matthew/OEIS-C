/*
 * A055137  Regard triangle of rencontres numbers (see A008290) as infinite
 *          matrix, compute inverse, read by rows.
 * https://oeis.org/A055137
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, tabl
 * Formula:   a(n) = (1 - (r - c)) * C(r, c) where r =
 *            floor((sqrt(8*n+1)-1)/2) is the row number, c = n - r*(r+1)/2 is
 *            the column number
 * Range:     i8 n <= 37, i16 n <= 58, i32 n <= 58, i64 n <= 58, i128 n <= 58,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    /* Find the row r containing index n. */
    idx_t r = 0;
    /* Increase r while the cumulative count does not exceed n. */
    while ((r + 1) * (r + 2) / 2 <= n) r++;

    /* Column within the row. */
    idx_t c = n - r * (r + 1) / 2;

    /* Compute the binomial coefficient C(r, c). */
    seq_t bin = binomial(r, c);

    /* Compute the linear factor (1 - (r - c)). */
    seq_t factor = (seq_t)1 - (seq_t)(r - c);

    /* Return the product. */
    return bin * factor;
}
