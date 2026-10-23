/*
 * A121334  Triangle, read by rows, where T(n,k) = C( n*(n+1)/2 + n-k, n-k),
 *          for n>=k>=0.
 * https://oeis.org/A121334
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = binom( row*(row+1)/2 + (row - col), row - col ) where row
 *            = floor((sqrt(8*n+1)-1)/2) and col = n - row*(row+1)/2
 * Range:     u8 n <= 9, u16 n <= 14, u32 n <= 44, u64 n <= 45, u128 n <= 45,
 *            big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <math.h>
#include "../utils/binom.h"

seq_t a(idx_t n) {
    /* find the row that contains index n */
    idx_t row = (idx_t)((sqrt(8.0 * n + 1.0) - 1.0) / 2.0);
    /* correct any rounding error */
    while ((row + 1) * (row + 2) / 2 <= n) row++;
    while (row * (row + 1) / 2 > n) row--;

    idx_t offset = row * (row + 1) / 2;
    idx_t col = n - offset;                /* 0 ≤ col ≤ row */

    /* compute the binomial coefficient arguments */
    idx_t triangular = row * (row + 1) / 2; /* row*(row+1)/2 */
    idx_t N = triangular + (row - col);    /* numerator */
    idx_t K = row - col;                   /* denominator */

    return binom(N, K);
}
