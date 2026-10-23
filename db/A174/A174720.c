/*
 * A174720  Triangle T(n, k, q) = (1-q^n)*( binomial(n, k) - 1 ) + 1, with q =
 *          4, read by rows.
 * https://oeis.org/A174720
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, tabl
 * Formula:   a(n) = (1-4^row)*(C(row,col)-1)+1, where row = ⌊(√(8·n+1)-1)/2⌋
 *            and col = n - row·(row+1)/2 (q = 4)
 * Range:     i8 n <= 7, i16 n <= 22, i32 n <= 80, i64 n <= 262,
 *            i128 n <= 1006, big any n
 * Helpers:   binomial, floor_sqrt, pow4
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/floor_sqrt.h"
#include "../utils/binomial.h"
#include "../utils/pow4.h"

seq_t a(idx_t n)
{
    /* 8·n + 1 as seq_t to avoid overflow of the index type. */
    seq_t t = (seq_t)n * 8 + 1;

    /* Floor of the square root.  The function is guaranteed to return
       a seq_t value. */
    seq_t sqrt_t = floor_sqrt(t);

    /* Compute row. */
    idx_t row = ((idx_t)(sqrt_t - 1)) / 2;

    /* Number of terms before this row – the triangular number. */
    idx_t pref = row * (row + 1) / 2;

    /* Column index within the row (0‑based). */
    idx_t col = n - pref;

    /* Binomial coefficient C(row, col). */
    seq_t bin = binomial(row, col);

    /* 4^row. */
    seq_t pow4_row = pow4(row);

    /* Final value of the triangle entry. */
    seq_t result = (1 - pow4_row) * (bin - 1) + 1;

    return result;
}
