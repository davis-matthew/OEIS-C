/*
 * A230419  Square array A(n,k) = difference of digit sums in factorial base
 *          representations (A007623) of n and k, n>=0, k>=0, read by
 *          antidiagonals; A(n,k) = A034968(n)-A034968(k).
 * https://oeis.org/A230419
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, base, tabl
 * Formula:   a(n) = A034968(col(n)) - A034968(row(n)) where
 *            (col,row)=antidiagonal_coords(n)
 * Range:     i8 n <= 7380, i16 n <= 7380, i32 n <= 7380, i64 n <= 7380,
 *            i128 n <= 7380, big any n
 * Helpers:   antidiagonal_coords, factoradic_digit_sum
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/antidiagonal_coords.h"
#include "../utils/factoradic_digit_sum.h"

/* Sequence A230419 */
seq_t a(idx_t n)
{
    idx_t row, col;
    antidiagonal_coords(n, &row, &col);          /* row = i, col = j as per OEIS definition */

    seq_t sum_col = factoradic_digit_sum(col);
    seq_t sum_row = factoradic_digit_sum(row);

    return sum_col - sum_row;
}
