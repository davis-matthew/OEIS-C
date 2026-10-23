/*
 * A124645  Triangle T(n,k), 0<=k<=n, read by rows given by
 *          [1,-1,0,0,0,0,0,...] DELTA [ -1,2,-1,0,0,0,0,0,0,...] where DELTA
 *          is the operator defined in A084938 .
 * https://oeis.org/A124645
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, tabl
 * Formula:   a(n) = T(r,k) where r = floor((sqrt(8*n+1)-1)/2), k = n -
 *            r(r+1)/2, T(r,k) = (-1)^(k+floor(r/2)) *
 *            binomial(floor((r+1)/2), k-floor(r/2))
 * Range:     i8 n <= 163, i16 n <= 653, i32 n <= 1325, i64 n <= 1325,
 *            i128 n <= 1325, big any n
 * Helpers:   binom, offset_row
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/offset_row.h"
#include "../utils/binom.h"

seq_t a(idx_t n)
{
    /* Locate the row r containing index n. */
    idx_t r = 0;
    while (offset_row(r + 1) <= n)
        ++r;

    idx_t row_start = offset_row(r);   /* first index of row r */
    idx_t k = n - row_start;           /* column within the row */

    idx_t floor_r2 = r / 2;             /* floor(r/2) */
    if (k < floor_r2)
        return 0;                       /* binomial is zero for negative lower argument */

    idx_t kk = k - floor_r2;            /* k - floor(r/2) */
    idx_t binom_n = (r + 1) / 2;         /* floor((r+1)/2) */

    if (kk > binom_n)
        return 0;                       /* binomial is zero when kk > binom_n */

    seq_t val = binom(binom_n, kk);     /* binomial coefficient */

    /* Apply the alternating sign: (-1)^(k + floor(r/2)) */
    if ( ((k + floor_r2) & (idx_t)1) == 1 )
        val = -val;

    return val;
}
