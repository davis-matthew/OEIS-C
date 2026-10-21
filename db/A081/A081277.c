/*
 * A081277  Square array of unsigned coefficients of Chebyshev polynomials of
 *          the first kind.
 * https://oeis.org/A081277
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 23, u16 n <= 63, u32 n <= 63, u64 n <= 63, u128 n <= 63,
 *            big any n
 * Helpers:   binomial, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation computes the OEIS sequence A081277.
   The sequence is obtained by flattening the infinite array
   T(row,col) of unsigned coefficients of Chebyshev
   polynomials of the first kind along its antidiagonals.
   For row,col ≥ 0 and col>0, row>0,
        T(row,col) = (col + 2*row) * binomial(col+row-1, row-1) *
                     2^(col-1) / row
   with the conventions
        T(row,0) = 1      for all row
        T(0,col) = 2^(col-1)   for col > 0
   The index n is mapped to the pair (row,col) by
        d = ⌊(-1 + √(1+8n))/2⌋   (the diagonal number)
        offset = n - d(d+1)/2
        row = d - offset
        col = offset
   The function below therefore first computes (row,col) from n
   and then evaluates T(row,col) using the formula above. */

#include <stdint.h>
#include "../utils/binomial.h"
#include "../utils/pow2.h"

/* Return the largest d with d*(d+1)/2 <= n.  */
static idx_t triangular_index(idx_t n)
{
    idx_t lo = 0, hi = n;
    while (lo < hi) {
        idx_t mid = (lo + hi + 1) >> 1;
        __int128 val = (__int128)mid * (mid + 1) / 2;
        if (val <= n)
            lo = mid;
        else
            hi = mid - 1;
    }
    return lo;
}

/* Compute the array entry T(row, col).  */
static seq_t compute_T(idx_t row, idx_t col)
{
    /* First column is always 1. */
    if (col == 0)
        return (seq_t)1;

    /* Row 0: 2^(col-1) for col > 0. */
    if (row == 0)
        return pow2(col - 1);

    /* General case (row > 0, col > 0).  */
    seq_t bin    = binomial(col + row - 1, row - 1);
    seq_t mult   = (seq_t)(col + 2 * row);
    seq_t pw     = pow2(col - 1);
    seq_t result = bin;
    result *= mult;
    result *= pw;
    result /= (seq_t)row;
    return result;
}

/* Public routine computing a(n).  */
seq_t a(idx_t n)
{
    idx_t d      = triangular_index(n);                     /* diagonal number */
    __int128 td  = (__int128)d * (d + 1) / 2;                /* sum of first d diagonals */
    idx_t offset = n - (idx_t)td;                           /* offset within diagonal */
    idx_t row    = d - offset;                               /* row index (i) */
    idx_t col    = offset;                                   /* column index (j) */
    return compute_T(row, col);
}
