/*
 * A133607  Triangle read by rows: T(n, k) = qStirling2(n, k, q) for q = -1,
 *          with 0 <= k <= n.
 * https://oeis.org/A133607
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, tabl
 * Formula:   T(n,k) = T(n-1,k) - T(n-2,k-2) * with T(0,0)=1; * T(1,0)=0,
 *            T(1,1)=1; * T(2,0)=0, T(2,1)=1, T(2,2)=-1; * and T(n,k)=0 if k<0
 *            or k>n. * The sequence a(n) is the flattened triangle read by
 *            rows, * i.e. a(n)=T(row, col) where n = row*(row+1)/2 + col with
 *            0≤col≤row
 * Range:     i8 n <= 77, i16 n <= 77, i32 n <= 77, i64 n <= 77, i128 n <= 77,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Determine which row contains the n‑th element. */
    idx_t r = 0;
    idx_t cum = 0;                  /* number of elements before the current row */
    while (cum + (r + 1) <= n) {
        cum += r + 1;
        r++;
    }
    idx_t k = n - cum;               /* column index within the row */

    if (r == 0) return (seq_t)1;     /* row 0: only one element */

    /* Allocate the first two rows. */
    seq_t *row_nm2 = (seq_t*)malloc(sizeof(seq_t));
    row_nm2[0] = (seq_t)1;           /* T(0,0) */

    seq_t *row_nm1 = (seq_t*)malloc(2 * sizeof(seq_t));
    row_nm1[0]  = (seq_t)0;          /* T(1,0) */
    row_nm1[1]  = (seq_t)1;          /* T(1,1) */

    if (r == 1) {                    /* already have the needed row */
        seq_t res = row_nm1[k];
        free(row_nm2);
        free(row_nm1);
        return res;
    }

    /* Build rows incrementally up to row r. */
    seq_t *cur = NULL;
    for (idx_t i = 2; i <= r; ++i) {
        size_t sz = (size_t)(i + 1);
        cur = (seq_t*)malloc(sz * sizeof(seq_t));

        for (idx_t j = 0; j <= i; ++j) {
            seq_t a1 = (j <= (i - 1)) ? row_nm1[j] : (seq_t)0;
            seq_t a2 = (j >= 2) ? row_nm2[j - 2] : (seq_t)0;
            cur[j] = a1 - a2;
        }

        free(row_nm2);
        row_nm2 = row_nm1;
        row_nm1 = cur;
    }

    seq_t result = row_nm1[k];
    free(row_nm2);
    free(row_nm1);
    return result;
}
