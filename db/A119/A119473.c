/*
 * A119473  Triangle read by rows: T(n,k) is number of binary words of length
 *          n and having k runs of 0's of odd length, 0 <= k <= ceiling(n/2).
 *          (A run of 0's is a subsequence of consecutive 0's of maximal
 *          length.)
 * https://oeis.org/A119473
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   not stated
 * Range:     u8 n <= 35, u16 n <= 101, u32 n <= 345, u64 n <= 1198,
 *            u128 n <= 4506, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Triangle T(n,k) with k runs of odd‑length 0s:
   T(0,0)=1,
   T(1,0)=T(1,1)=1,
   T(n,k)=T(n-1,k)+T(n-2,k)+T(n-2,k-1)   (for n≥2).
   a(idx_t n) returns the n‑th element of this triangle when it
   is flattened row by row (row 0, row 1, …).  For example
   a(0)=1, a(1)=1, a(2)=1, a(3)=2, a(4)=2, a(5)=3, …   */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Static storage for the computed rows and cumulative counts */
    static seq_t **triangle   = NULL;   /* pointers to each row */
    static idx_t *cum         = NULL;   /* cumulative element counts */
    static size_t  rows_cap   = 0;      /* allocated capacity for rows/memo */
    static size_t  rows_used  = 0;      /* number of rows currently stored */

    /* Expand the table until it contains the requested index n */
    while (rows_used == 0 || cum[rows_used - 1] <= n)
    {
        /* Determine row index and number of elements */
        idx_t  r        = (idx_t)rows_used;       /* next row to compute */
        idx_t  kmax     = (r + 1) / 2;            /* ceil(r/2) */
        size_t row_len  = (size_t)(kmax + 1);     /* number of k values 0..kmax */

        /* Allocate memory for the new row, expanding capacity if needed */
        if (rows_used >= rows_cap)
        {
            size_t new_cap = rows_cap == 0 ? 4 : rows_cap * 2;
            seq_t **tmp_rows = (seq_t**)realloc(triangle, new_cap * sizeof(seq_t*));
            if (!tmp_rows) return 0;          /* allocation failure → return 0 */
            idx_t *tmp_cum   = (idx_t*)realloc(cum,     new_cap * sizeof(idx_t));
            if (!tmp_cum)   return 0;
            triangle = tmp_rows;
            cum      = tmp_cum;
            rows_cap = new_cap;
        }

        seq_t *row = (seq_t*)malloc(row_len * sizeof(seq_t));
        if (!row) return 0; /* allocation failure → return 0 */

        /* Compute the row entries */
        if (r == 0)
        {
            row[0] = 1;
        }
        else if (r == 1)
        {
            row[0] = 1;
            row[1] = 1;
        }
        else
        {
            idx_t kmax_prev1 = ((r - 1) + 1) / 2; /* ceil((r-1)/2) */
            idx_t kmax_prev2 = ((r - 2) + 1) / 2; /* ceil((r-2)/2) */

            for (idx_t k = 0; k <= kmax; ++k)
            {
                seq_t val = 0;

                if (k <= kmax_prev1)
                    val += triangle[r - 1][k];
                if (k <= kmax_prev2)
                    val += triangle[r - 2][k];
                if (k >= 1 && (k - 1) <= kmax_prev2)
                    val += triangle[r - 2][k - 1];

                row[k] = val;
            }
        }

        triangle[rows_used] = row;
        cum[rows_used] = (rows_used == 0) ? row_len : cum[rows_used - 1] + row_len;
        ++rows_used;
    }

    /* Locate the row that contains index n */
    idx_t low = 0, high = (idx_t)(rows_used - 1);
    while (low < high)
    {
        idx_t mid = (low + high) >> 1;
        if (cum[mid] > n)
            high = mid;
        else
            low  = mid + 1;
    }
    idx_t row_index  = low;
    idx_t prev_start = (row_index == 0) ? 0 : cum[row_index - 1];
    idx_t k          = n - prev_start;

    return triangle[row_index][k];
}
