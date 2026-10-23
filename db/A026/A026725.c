/*
 * A026725  Triangular array, T, read by rows: T(n,0) = T(n,n) = 1. For n >= 2
 *          and 1<=k<=n-1, T(n,k) = T(n-1,k-1) + T(n-2,k-1) + T(n-1,k) if n is
 *          odd and k=(n-1)/2, otherwise T(n,k) = T(n-1,k-1) + T(n-1,k).
 * https://oeis.org/A026725
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = T(r,k) where r = floor((sqrt(8n+1)-1)/2) and k = n -
 *            r*(r+1)/2. T is defined by T(0,0)=1, T(n,0)=T(n,n)=1,
 *            T(n,k)=T(n-1,k-1)+T(n-1,k) if n is even or k!=(n-1)/2
 *            T(n,k)=T(n-1,k-1)+T(n-2,k-1)+T(n-1,k) if n is odd and k==(n-1)/2
 *            (Read the triangle row‑by‑row, a(0) is the first entry of row 0)
 * Range:     u8 n <= 48, u16 n <= 160, u32 n <= 575, u64 n <= 2046,
 *            u128 n <= 5049, big any n
 * Helpers:   alloc_seq_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

/* Integer square‑root (floor) for idx_t.  No overflow for idx_t up to 2^64-1. */
static idx_t isqrt_u64(idx_t x)
{
    idx_t low = 0;
    idx_t high = x;
    idx_t ans = 0;
    while (low <= high) {
        idx_t mid = low + ((high - low) >> 1);
        if (mid == 0 || mid <= x / mid) {
            ans = mid;
            low = mid + 1;
        } else {
            high = mid - 1;
        }
    }
    return ans;
}

/* Static storage for the computed rows of the triangle.
   row_ptrs[i] points to an array of length i+1 holding T(i,k).  */
static seq_t **row_ptrs = NULL;      /* array of row pointers        */
static size_t  row_cap  = 0;         /* capacity of the row_ptrs array */
static idx_t   row_cnt  = 0;         /* number of rows currently stored */

/* Grow the array that stores pointers to rows. */
static void grow_rows_array(void)
{
    size_t new_cap = (row_cap == 0) ? 4 : row_cap * 2;
    seq_t **p = (seq_t **)realloc(row_ptrs, new_cap * sizeof(seq_t *));
    if (!p) exit(1);
    row_ptrs = p;
    row_cap = new_cap;
}

/* Allocate an array of seq_t elements.  The utility alloc_seq_array is provided
   in the build environment.  It is declared in "../utils/alloc_seq_array.h". */
#include "../utils/alloc_seq_array.h"

/* Compute the row r of the triangle, storing it in row_ptrs. */
static void compute_row(idx_t r)
{
    /* Allocate memory for the new row (length r+1). */
    seq_t *arr = alloc_seq_array((size_t)(r + 1));
    if (!arr) exit(1);

    /* Boundaries: T(r,0)=T(r,r)=1. */
    arr[0] = (seq_t)1;
    arr[r] = (seq_t)1;

    /* Inner elements using recurrence. */
    for (idx_t k = 1; k < r; ++k) {
        if (((r & 1U) != 0U) && (k == (r - 1) / 2)) {
            /* Special odd rule. Requires rows r-1 and r-2. */
            arr[k] = row_ptrs[r - 1][k - 1] + row_ptrs[r - 2][k - 1] + row_ptrs[r - 1][k];
        } else {
            /* Normal rule. Requires only row r-1. */
            arr[k] = row_ptrs[r - 1][k - 1] + row_ptrs[r - 1][k];
        }
    }

    /* Store the new row. */
    if (row_cnt >= row_cap) grow_rows_array();
    row_ptrs[row_cnt++] = arr;
}

/* Ensure that all rows up to and including 'upto' are computed. */
static void ensure_rows(idx_t upto)
{
    for (idx_t r = row_cnt; r <= upto; ++r)
        compute_row(r);
}

/* Return the value of the sequence at index n. */
seq_t a(idx_t n)
{
    /* Find the row r and column k corresponding to flattened index n. */
    idx_t r = (isqrt_u64(8 * n + 1) - 1) / 2;
    idx_t start = r * (r + 1) / 2;   /* first index of row r */
    idx_t k = n - start;             /* column position within the row */

    /* Compute rows as needed. */
    if (r >= row_cnt) ensure_rows(r);

    return row_ptrs[r][k];
}
