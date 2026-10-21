/*
 * A200756  Triangle T(n,k) = coefficient of x^n in expansion of ((1 -sqrt(1 -
 *          4*x - 4*x^2))/2)^k.
 * https://oeis.org/A200756
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 10, u16 n <= 36, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A200756: T(n,k) = coefficient of x^n in ((1-√(1-4x-4x^2))/2)^k
    a(n) reads the rows of T left to right.  */
#include <stddef.h>
#include <stdint.h>
#include "../utils/binom.h"

static seq_t T_row_col(idx_t row, idx_t col)
{
    seq_t sum = (seq_t)0;
    for (idx_t i = col; i <= row; ++i) {
        seq_t term = binom(i, row - i);                           /* binom(i,n-i) */
        term = term * binom(2 * i - col - 1, i - 1);             /* binom(2i-k-1,i-1) */
        term = term * (seq_t)col;                                /* multiply by k before division */
        term /= i;                                                /* safe integer division */
        sum += term;
    }
    return sum;
}

seq_t a(idx_t n)
{
    if (n == (idx_t)0) return (seq_t)0;                          /* offset 1 */

    /* Find the row that contains the n-th element */
    idx_t row = 1;
    while (row * (row + 1) / 2 < n) ++row;

    /* Column index within that row */
    idx_t col = n - (row - 1) * row / 2;   /* 1 <= col <= row */

    return T_row_col(row, col);
}
