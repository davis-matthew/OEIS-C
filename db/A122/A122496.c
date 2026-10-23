/*
 * A122496  T(n, m) = 2^m * binomial(-m, n), for 0 <= m <= n, n >= 0, triangle
 *          read by rows.
 * https://oeis.org/A122496
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, easy, tabl
 * Formula:   a(n) = value obtained by flattening the triangle T(row, col) =
 *            2^col * binomial(-col, row) row‑by‑row. The function computes
 *            the n‑th term (a(1) = 1)
 * Range:     i8 n <= 14, i16 n <= 34, i32 n <= 90, i64 n <= 299,
 *            i128 n <= 1034, big any n
 * Helpers:   binomial, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"
#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    /* n >= 1 by OEIS offset; behaviour for n < 1 is irrelevant */
    if (n < 1) return 0;

    /* Find the row that contains position n by accumulating lengths. */
    idx_t row   = 0;
    idx_t cum   = 1;          /* number of elements up to current row   */
    while (n > cum) {
        row++;
        cum += row + 1;       /* next row has length row+1             */
    }
    /* Elements before this row: cum - (row+1) */
    idx_t before = cum - (row + 1);
    idx_t col    = n - before - 1;   /* column index within the row   */

    /* Handle the first column separately (m = 0) */
    if (col == 0) {
        return (row == 0) ? (seq_t)1 : (seq_t)0;
    }

    /* For col > 0:
       T(row, col) = 2^col * binomial(-col, row)
                = 2^col * (-1)^row * binomial(col + row - 1, row)   */
    seq_t val = pow2(col);
    val = val * binomial(col + row - 1, row);
    if (row & 1) {          /* apply the (-1)^row factor */
        val = -val;
    }
    return val;
}
