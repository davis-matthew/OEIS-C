/*
 * A143199  Triangle read by rows: T(n, k) = (n+1)*A000096(k-1) + n if k <=
 *          floor(n/2), otherwise T(n, k) = (n+1)*A000096(n-k-1) + n.
 * https://oeis.org/A143199
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, tabl
 * Formula:   a(n) = row + (row+1)*((col-1)*(col+2)/2) * where (row,col) are
 *            the row and column of n in the triangle * T(row,col) defined in
 *            the OEIS entry (col=0..row). * The row is the largest r with
 *            r*(r+1)/2 <= n. * col = n - r*(r+1)/2, and the formula is
 *            symmetric: if col>r/2 use col=r-col
 * Range:     i8 n <= 59, i16 n <= 1325, i32 n <= 1325, i64 n <= 1325,
 *            i128 n <= 1325, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <math.h>

seq_t a(idx_t n)
{
    /* find the row r such that r*(r+1)/2 <= n < (r+1)*(r+2)/2 */
    /* using the integer formula r = floor((sqrt(8*n+1)-1)/2) with corrections */
    double d = sqrt((double)8 * n + 1.0);
    idx_t r = (idx_t)((d - 1.0) * 0.5);   /* initial guess */

    /* correct for possible rounding errors */
    while ((r + 1) * (r + 2) / 2 <= n) r++;
    while (r * (r + 1) / 2 > n) r--;

    /* column index within the row */
    idx_t col = n - r * (r + 1) / 2;

    /* use symmetry to keep col <= r/2 */
    if (col > r / 2) col = r - col;

    /* compute the value using the explicit formula */
    seq_t row = (seq_t)r;
    seq_t km   = (seq_t)col - 1;          /* (col-1) as signed */
    seq_t add  = km * ((seq_t)col + 2) / 2; /* ((col-1)*(col+2))/2 */
    seq_t val  = row + (row + 1) * add;

    return val;
}
