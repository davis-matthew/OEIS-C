/*
 * A153821  Recursive triangular sequence: A(n,k)= A(n - 1, k - 1) + A(n - 1,
 *          k) + (3 (-1 + n) (-4 + 3 n))*A(n - 2, k - 1).
 * https://oeis.org/A153821
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 17, u32 n <= 43, u64 n <= 43, u128 n <= 43,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: A(1,1)=2; A(2,1)=5; A(2,2)=5;
   A(3,1)=2; A(3,2)=76; A(3,3)=2;
   For n ≥ 4 and 1 < k < n:
     A(n,k)=A(n-1,k-1)+A(n-1,k)+3*(n-1)*(3*n-4)*A(n-2,k-1).
   The sequence a(n) is obtained by reading the triangle row by row
   (left‑to‑right, starting with n = 1). */

#include <stdlib.h>

/* static caches for rows of the triangular array */
static seq_t **row_data   = NULL;   /* row_data[n][k] , 1‑based indices   */
static idx_t *cum_entries = NULL;   /* cumulative entry counts, cum_entries[0] = 0 */
static idx_t max_row      = 0;       /* highest row cached                 */
static idx_t total_entries= 0;       /* total number of terms cached       */

/* ensure rows up to the smallest r with total_entries >= n */
static void ensure_rows(idx_t n)
{
    /* first initialisation */
    if (max_row == 0) {
        row_data   = (seq_t**)realloc(row_data,   1 * sizeof(seq_t*)); /* index 0 unused */
        cum_entries= (idx_t*)realloc(cum_entries,1 * sizeof(idx_t));
        row_data[0]   = NULL;
        cum_entries[0]= 0;
    }

    while (total_entries < n) {
        idx_t new_row = max_row + 1;
        seq_t *row = (seq_t*)malloc((new_row + 1) * sizeof(seq_t));
        if (!row) abort();   /* allocation failure */

        if (new_row == 1) {
            row[1] = (seq_t)2;
        } else if (new_row == 2) {
            row[1] = (seq_t)5;
            row[2] = (seq_t)5;
        } else if (new_row == 3) {
            row[1] = (seq_t)2;
            row[2] = (seq_t)76;
            row[3] = (seq_t)2;
        } else {
            seq_t *prev1 = row_data[new_row - 1];
            seq_t *prev2 = row_data[new_row - 2];
            seq_t coef = (seq_t)3 *
                         ( (seq_t)(new_row - 1) ) *
                         ( (seq_t)(3 * new_row - 4) );

            row[1] = prev1[1];
            for (idx_t k = 2; k < new_row; ++k) {
                row[k] = prev1[k - 1] + prev1[k] + coef * prev2[k - 1];
            }
            row[new_row] = prev1[new_row - 1];
        }

        /* extend the row array */
        row_data = (seq_t**)realloc(row_data,(new_row + 1) * sizeof(seq_t*));
        row_data[new_row] = row;

        /* update cumulative counts */
        cum_entries = (idx_t*)realloc(cum_entries,(new_row + 1) * sizeof(idx_t));
        cum_entries[new_row] = total_entries + new_row;

        /* update state */
        max_row      = new_row;
        total_entries= cum_entries[new_row];
    }
}

seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;           /* out of domain; safe fallback */

    /* build cache up to n */
    ensure_rows(n);

    /* find the row containing position n */
    idx_t r = 1;
    while (cum_entries[r] < n) ++r;

    /* index within that row (1‑based) */
    idx_t pos = n - cum_entries[r - 1];
    return row_data[r][pos];
}
