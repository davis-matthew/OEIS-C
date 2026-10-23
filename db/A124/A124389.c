/*
 * A124389  A square array of Kolakoski string lengths, read by antidiagonals.
 * https://oeis.org/A124389
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 65, u16 n <= 65, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A124389 – antidiagonal read‑off of the Kolakoski‑string‑length
   square array.  For row k the entry a_m satisfies
        a_0 = a_1 = a_2 = 1,
        a_m = (k+1)*a_{m-2} + k*a_{m-3}   (m ≥ 3).
   This routine computes A(n) for any n ≥ 0 by
   (1) locating the unique triangle index d with d(d+1)/2 ≤ n < (d+1)(d+2)/2,
   (2) determining the cell (i,j) on that diagonal (i = row, j = column),
   (3) evaluating the k‑th row up to column j using the above recurrence.  */

#include <stdbool.h>

/* Main sequence function --------------------------------------------------- */
seq_t a(idx_t n)
{
    /* Step 1: find the diagonal index d (triangular coordinates) */
    idx_t d = 0;
    while ((d + 1) * (d + 2) / 2 <= n) {
        ++d;
    }

    /* Step 2: offset of n along this diagonal */
    idx_t offset = n - d * (d + 1) / 2;
    idx_t i = offset;            /* row index  (k)  */
    idx_t j = d - offset;        /* column index (m) */

    /* Step 3: evaluate row i up to column j using the recurrence */
    if (j <= 2) {                       /* a_0, a_1, a_2 are always 1 */
        return (seq_t)1;
    }

    /* Initial values for m = 0,1,2 */
    seq_t a_nm3 = (seq_t)1;   /* a_{m-3}  */
    seq_t a_nm2 = (seq_t)1;   /* a_{m-2}  */
    seq_t a_nm1 = (seq_t)1;   /* a_{m-1}  */

    seq_t new_val;
    for (idx_t m = 3; m <= j; ++m) {
        new_val = (seq_t)(i + 1) * a_nm2 + (seq_t)i * a_nm3;
        a_nm3 = a_nm2;
        a_nm2 = a_nm1;
        a_nm1 = new_val;
    }

    return a_nm1;   /* a_j of row i */
}
