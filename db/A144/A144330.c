/*
 * A144330  Triangle read by rows, A144328 * A000012 as infinite lower
 *          triangular matrices.
 * https://oeis.org/A144330
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   a(n) = T(row,col) where * row = floor((sqrt(8*n+1)-1)/2), * col
 *            = n - row*(row+1)/2. * T(row,0) = row*(row+1)/2 + 1, *
 *            T(row,col>0) = row*(row+1)/2 - col*(col-1)/2
 * Range:     u8 n <= 275, u16 n <= 1325, u32 n <= 1325, u64 n <= 1325,
 *            u128 n <= 1325, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <math.h>


seq_t a(idx_t n)
{
    /* find the row of the triangle */
    long double val = sqrt((long double)8 * (long double)n + 1.0L);
    idx_t row = (idx_t)((val - 1.0L) / 2.0L);

    /* adjust in case the approximation is off by 1 */
    idx_t off = row * (row + 1) / 2;
    while (off > n) {
        row--;
        off = row * (row + 1) / 2;
    }
    while ((row + 1) * (row + 2) / 2 <= n) {
        row++;
    }

    idx_t col = n - off;

    seq_t tri = ((seq_t)row * ((seq_t)row + 1)) / 2;

    if (col == 0) {
        return tri + 1;
    } else {
        seq_t binom = ((seq_t)col * ((seq_t)col - 1)) / 2;
        return tri - binom;
    }
}
