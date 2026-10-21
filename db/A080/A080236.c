/*
 * A080236  Triangle of differences of consecutive pairs of row elements of
 *          triangle A080233.
 * https://oeis.org/A080236
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, tabl
 * Formula:   not stated
 * Range:     i8 n <= 65, i16 n <= 65, i32 n <= 65, i64 n <= 65, i128 n <= 65,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   Computes OEIS sequence A080236:
   Triangle of differences of consecutive pairs of row elements
   of triangle A080233.
   ------------------------------------------------------------------
   The triangle satisfies the following recurrence for 0 ≤ k < r:
       T(r,0)          = 1
       T(r,k)          = T(r-1,k) + T(r-1,k-1)
   The last element of each row is determined from the row sum
       Σ_{k=0}^{r} T(r,k) = 1 - r
   Hence, after computing T(r,k) for 0 ≤ k < r, the last entry is
   set to (1 - r) minus the partial sum of the first r entries.
   ------------------------------------------------------------------
   Access pattern:
        a(n) = element with linear index n (row-major order,
        starting with the first row: {1}).
   ------------------------------------------------------------------
   The implementation builds the triangle lazily with memoisation.
   It works for any integer type seq_t / idx_t provided by the
   build system.  No external functions are required.
*/

#include <stdlib.h>
#include <stdint.h>

/* Triangle stored in a flat array. Index of element (r, k) is
   offset(r) + k, where offset(r) = r*(r+1)/2. */
static seq_t *triangle = NULL;              /* flat storage */
static size_t triangle_size = 0;            /* total allocated elements */
static long long max_computed_row = -1;     /* highest row fully computed */

/* Ensure that the triangle has rows 0 … needed_row computed. */
static void
ensure_triangle(long long needed_row)
{
    if (needed_row <= max_computed_row)
        return;          /* nothing to do */

    /* Allocate a new flat array big enough for all rows up to needed_row. */
    size_t new_size = (size_t)(needed_row + 1) * (needed_row + 2) / 2;
    seq_t *tmp = (seq_t *)realloc(triangle, new_size * sizeof(seq_t));
    if (!tmp)
        abort();         /* allocation failed */
    triangle = tmp;
    triangle_size = new_size;

    /* Compute the missing rows. */
    for (long long r = max_computed_row + 1; r <= needed_row; ++r) {
        size_t start_idx = (size_t)r * (r + 1) / 2;          /* offset(r)   */
        /* Row 0 is the trivial case. */
        if (r == 0) {
            triangle[start_idx] = (seq_t)1;
            max_computed_row = 0;
            continue;
        }

        size_t prev_start = (size_t)(r - 1) * r / 2;          /* offset(r-1) */
        /* First entry in the row. */
        triangle[start_idx] = (seq_t)1;
        seq_t sum_partial   = triangle[start_idx];           /* sum of computed entries */

        /* Compute entries 1 … r-1 using the recurrence. */
        for (long long k = 1; k < r; ++k) {
            seq_t val = triangle[prev_start + k]          /* T(r-1,k)   */
                       + triangle[prev_start + k - 1];   /* + T(r-1,k-1) */
            triangle[start_idx + k] = val;
            sum_partial += val;
        }

        /* Last entry from the row-sum condition. */
        seq_t row_sum_needed = (seq_t)1 - (seq_t)r;          /* 1 - r   */
        triangle[start_idx + r] = row_sum_needed - sum_partial;

        max_computed_row = r;
    }
}

/* Main interface as required. */
seq_t
a(idx_t n)
{
    /* Find the row r such that n lies in that row. */
    long long r = 0;                                /* row index  */
    while ((size_t)(r + 1) * (r + 2) / 2 <= n)
        ++r;

    /* Build the triangle up to this row if necessary. */
    ensure_triangle(r);

    /* Compute flat index and return the element. */
    size_t start = (size_t)r * (r + 1) / 2;          /* offset(r)   */
    size_t idx   = start + (size_t)(n - (idx_t)start);
    return triangle[idx];
}
