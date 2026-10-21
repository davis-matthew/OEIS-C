/*
 * A237448  Square array T(row >= 1, col >= 1): The first row, row=1, T(1,col)
 *          = col = A000027. When row > col, T(row,col) = row, otherwise (when
 *          1 < row <= col), T(row,col) = row-1.
 * https://oeis.org/A237448
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   a(n) = T(row,column) where T(row,col) = col if row==1 else = row
 *            if row>col else row-1. The sequence is obtained by enumerating
 *            the infinite square array along antidiagonals starting at (1,1)
 * Range:     u8 n <= 9923, u16 n <= 9923, u32 n <= 9923, u64 n <= 9923,
 *            u128 n <= 9923, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <math.h>

seq_t a(idx_t n)
{
    /* Compute diagonal index d such that n lies in diagonal d
       (sum of first d natural numbers S_d = d*(d+1)/2). */
    double root = sqrt((double)8 * (double)n + 1.0);
    idx_t d = (idx_t)((root - 1.0) / 2.0);          /* floor approximation */
    /* Adjust d to be the smallest integer with S_d >= n */
    __int128 sd = (__int128)d * (d + 1) / 2;
    if (sd < n) {
        ++d;
    } else {
        __int128 s_prev = (__int128)(d - 1) * d / 2;
        if (s_prev >= n) {
            --d;
        }
    }
    __int128 prev = (__int128)d * (d - 1) / 2;
    idx_t offset = (idx_t)(n - (idx_t)prev - 1);   /* zero-based offset within diagonal */
    idx_t row = offset + 1;
    idx_t col = d - offset;

    if (row == 1) {
        return (seq_t)col;
    } else if (row > col) {
        return (seq_t)row;
    } else {
        return (seq_t)(row - 1);
    }
}
