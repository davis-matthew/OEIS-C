/*
 * A144944  Super-Catalan triangle (read by rows) = triangular array
 *          associated with little Schroeder numbers (read by rows): T(0,0)=1,
 *          T(p,q) = T(p,q-1) if 0 < p = q, T(p,q) = T(p,q-1) + T(p-1,q) +
 *          T(p-1,q-1) if -1 < p < q and T(p,q) = 0 otherwise.
 * https://oeis.org/A144944
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nice, nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 24, u16 n <= 52, u32 n <= 149, u64 n <= 461,
 *            u128 n <= 1591, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Super‑Catalan triangle (read by rows)
   a(n) = T(p, q) where
     q = floor( (sqrt(8*n+1)-1)/2 )
     p = n - q*(q+1)/2
   and T is defined by
     T(0,0) = 1
     T(p, q) = T(p, q-1)                if 0 < p = q
     T(p, q) = T(p, q-1)+T(p-1, q)+T(p-1, q-1)
               if -1 < p < q
     T(p, q) = 0                        otherwise
*/

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

/* static storage for rows already computed */
static seq_t **row_ptrs   = NULL;  /* array of pointers to each row */
static idx_t rows_cap     = 0;      /* allocated capacity of row_ptrs */
static idx_t rows_len     = 0;      /* number of rows currently stored */

/* Ensure row_ptrs has space for at least 'need' rows */
static void ensure_rows_cap(idx_t need)
{
    if (need <= rows_cap) return;
    idx_t new_cap = rows_cap ? rows_cap << 1 : 8;
    while (new_cap < need) new_cap <<= 1;
    seq_t **tmp = (seq_t**)realloc(row_ptrs, new_cap * sizeof(seq_t*));
    if (!tmp) exit(EXIT_FAILURE);
    row_ptrs = tmp;
    rows_cap = new_cap;
}

/* Build rows up to 'target_row' inclusive, filling row_ptrs[0..target_row] */
static void build_rows_up_to(idx_t target_row)
{
    if (target_row < rows_len) return;          /* nothing to do */
    ensure_rows_cap(target_row + 1);

    for (idx_t r = rows_len; r <= target_row; ++r)
    {
        /* allocate current row (size r+1) */
        seq_t *row = (seq_t*)malloc((r + 1) * sizeof(seq_t));
        if (!row) exit(EXIT_FAILURE);

        for (idx_t p = 0; p <= r; ++p)
        {
            if (r == 0)            /* first row, only p==0 */
                row[p] = (seq_t)1;
            else if (p == 0)       /* first element of any row is 1 */
                row[p] = (seq_t)1;
            else if (p == r)       /* diagonal: copy left neighbour */
                row[p] = row[p-1];
            else                  /* interior element */
            {
                seq_t term1 = row_ptrs[r-1][p];     /* T(p,   r-1) */
                seq_t term2 = row[p-1];             /* T(p-1, r)   */
                seq_t term3 = row_ptrs[r-1][p-1];   /* T(p-1, r-1) */
                row[p] = term1 + term2 + term3;
            }
        }
        row_ptrs[r] = row;
    }
    rows_len = target_row + 1;
}

/* Find row index q such that q*(q+1)/2 <= n < (q+1)*(q+2)/2 */
static idx_t row_of_idx(idx_t n)
{
    idx_t low = 0, high = 1;
    while (high * (high + 1) / 2 <= n) high <<= 1;      /* find upper bound */
    while (low < high)
    {
        idx_t mid = low + (high - low + 1) / 2;
        if (mid * (mid + 1) / 2 <= n)
            low = mid;
        else
            high = mid - 1;
    }
    return low;
}

/* Public function: value a(n) of A144944 */
seq_t a(idx_t n)
{
    /* Determine the row (q) and column (p) of the n-th triangle entry */
    idx_t q = row_of_idx(n);
    idx_t p = n - q * (q + 1) / 2;

    /* Build rows up to q if needed */
    build_rows_up_to(q);

    /* Return the requested triangle entry */
    return row_ptrs[q][p];
}
