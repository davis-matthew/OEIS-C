/*
 * A156734  Square array read by antidiagonals up. T(n,k) = if k divides n
 *          then +1 else -1.
 * https://oeis.org/A156734
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, tabl
 * Formula:   a(n) = +1 if (col divides row) in the square array T(row,col),
 *            else -1; the array is read by antidiagonals up
 * Range:     i8 n <= 77, i16 n <= 77, i32 n <= 77, i64 n <= 77, i128 n <= 77,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <math.h>
#include <stdint.h>

/* Find the diagonal index d such that d*(d+1)/2 >= n.
   Diagonal 1 contains element 1, diagonal 2 contains elements 2,3, etc. */
static idx_t diag_index(idx_t n)
{
    /* Use long double sqrt for initial estimate, then correct. */
    long double d = sqrtl((long double)8.0L * n + 1.0L);
    idx_t diag = (idx_t)((d - 1.0L) / 2.0L);
    /* Correct possible rounding down. */
    if (((__uint128_t)diag * (diag + 1)) / 2 < n)
        diag++;
    return diag;
}

seq_t a(idx_t n)
{
    /* n >= 1, offset is 0-based. */
    idx_t diag = diag_index(n);
    idx_t prev = (diag - 1) * diag / 2;          /* total elements before this diag */
    idx_t offset = n - prev - 1;                 /* offset within the diag (0 … diag-1) */
    idx_t row = diag - offset;                   /* row index (1 … diag) */
    idx_t col = offset + 1;                      /* column index (1 … diag) */

    return (row % col == 0) ? (seq_t)1 : (seq_t)-1;
}
