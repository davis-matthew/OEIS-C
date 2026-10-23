/*
 * A027144  Triangular array T given by rows: T(n,0)=1 for n >= 0, T(1,1)=2;
 *          for even n >= 2, T(n,k)=T(n-2,k-1)+T(n-1,k-1)+T(n-1,k) for 1<=(odd
 *          k)<=n-1 and T(n,k)=T(n-1,k-1)+T(n-1,k) for 2<=(even k)<=n-2,
 *          T(n,n)=T(n-1,n-1); for odd n<=3,
 *          T(n,k)=T(n,k-1)+T(n-1,k-1)+T(n-1,k) for 1<=(odd k)<=n-2 and
 *          T(n,k)=T(n-...
 * https://oeis.org/A027144
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 33, u16 n <= 60, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A027144
   Triangular array T defined by:
   T(n,0)=1 (n≥0),      T(1,1)=2
   For even n≥2:
     if k even (2≤k≤n-2):       T(n,k)=T(n-1,k-1)+T(n-1,k)
     if k odd  (1≤k≤n-1):       T(n,k)=T(n-2,k-1)+T(n-1,k-1)+T(n-1,k)
     T(n,n)=T(n-1,n-1)
   For odd n (n≥1):
     if k even (2≤k≤n-1):       T(n,k)=T(n-1,k-1)+T(n-1,k)
     if k odd  (1≤k≤n-2):       T(n,k)=T(n,k-1)+T(n-1,k-1)+T(n-1,k)
     T(n,n)=T(n-1,n-1)+T(n,n-1)
   The sequence a(n) lists the entries of T row‑by‑row, left to right,
   starting with a(1)=T(0,0).  This function returns a(n) for any n≥1.
*/

#include <stdlib.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* Find row r and position k (0‑based) such that
       T(r,k) is the n‑th element of the flattened array. */
    idx_t r = 0;
    idx_t cum = 1;            /* total count up to row r (inclusive) */
    while (cum < n) {
        ++r;
        cum += (r + 1);       /* row r has r+1 entries */
    }
    idx_t prev_cum = cum - (r + 1);
    idx_t k = n - prev_cum - 1;   /* 0‑based column in row r */

    /* Compute rows up to r using dynamic programming. */
    seq_t *row_prevprev = NULL; /* row r-2       */
    seq_t *row_prev    = NULL; /* row r-1       */
    seq_t *row_curr    = NULL; /* row r         */

    /* Row 0 */
    row_prevprev = (seq_t *)malloc(sizeof(seq_t));
    row_prevprev[0] = 1;

    if (r == 0) {
        seq_t res = row_prevprev[0];
        free(row_prevprev);
        return res;
    }

    /* Row 1 */
    idx_t idx_row = 1;
    row_prev = (seq_t *)malloc((idx_row + 1) * sizeof(seq_t));
    row_prev[0] = 1;
    row_prev[1] = 2;
    if (r == 1) {
        seq_t res = row_prev[k];
        free(row_curr);
        free(row_prev);
        free(row_prevprev);
        return res;
    }

    /* Compute subsequent rows */
    for (idx_row = 2; idx_row <= r; ++idx_row) {
        row_curr = (seq_t *)malloc((idx_row + 1) * sizeof(seq_t));
        row_curr[0] = 1;

        bool even_n = (idx_row % 2 == 0);

        for (idx_t col = 1; col <= idx_row; ++col) {
            if (col == idx_row) {           /* last element of the row */
                if (even_n) {              /* even n: T(n,n)=T(n-1,n-1) */
                    row_curr[col] = row_prev[col - 1];
                } else {                    /* odd n: T(n,n)=T(n-1,n-1)+T(n,n-1) */
                    row_curr[col] = row_prev[col - 1] + row_curr[col - 1];
                }
            } else if (col % 2 == 1) {     /* odd column index */
                if (even_n) {
                    /* even n, odd k: T(n,k)=T(n-2,k-1)+T(n-1,k-1)+T(n-1,k) */
                    row_curr[col] = row_prevprev[col - 1] + row_prev[col - 1] + row_prev[col];
                } else {
                    /* odd n, odd k: T(n,k)=T(n,k-1)+T(n-1,k-1)+T(n-1,k) */
                    row_curr[col] = row_curr[col - 1] + row_prev[col - 1] + row_prev[col];
                }
            } else {                       /* even column index and col < n */
                /* even k: T(n,k)=T(n-1,k-1)+T(n-1,k) for both parities of n */
                row_curr[col] = row_prev[col - 1] + row_prev[col];
            }
        }

        /* Rotate rows: prevprev <- prev, prev <- curr */
        free(row_prevprev);
        row_prevprev = row_prev;
        row_prev     = row_curr;
    }

    /* After loop row_prev points to row r */
    seq_t result = row_prev[k];

    free(row_prev);
    free(row_prevprev);
    return result;
}
