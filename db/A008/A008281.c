/*
 * A008281  Triangle of Euler-Bernoulli or Entringer numbers read by rows.
 * https://oeis.org/A008281
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl, nice, easy, changed
 * Formula:   a(n) is the n-th element of the triangle of Entringer numbers *
 *            read by rows. The triangle T is defined by: * T(0,0) = 1 *
 *            T(n,0) = 0 (n > 0) * T(n,k) = T(n,k-1) + T(n-1, n-k) (k > 0)
 * Range:     u8 n <= 32, u16 n <= 67, u32 n <= 138, u64 n <= 326,
 *            u128 n <= 795, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Global state for memoisation – a flat array containing all rows
 * of the triangle concatenated.  flat_len holds the number of elements
 * stored, flat_cap the allocated capacity, and rows_computed the
 * largest row index that has been generated (row 0 is the first). */
static seq_t *flat_vals   = NULL;   /* Flattened triangle values      */
static size_t flat_cap    = 0;      /* Allocated capacity             */
static idx_t  flat_len    = 0;      /* Current number of elements     */
static idx_t  rows_computed = 0;    /* Highest row index fully computed */

/* Ensure that rows 0 .. r are computed and stored in flat_vals. */
static void
ensure_rows_up_to(idx_t r)
{
    while (rows_computed <= r) {
        idx_t row     = rows_computed;          /* current row to compute */
        idx_t cols    = row + 1;                /* number of elements in this row */

        /* Grow the flat array if necessary */
        if (flat_len + cols > flat_cap) {
            size_t new_cap = flat_cap ? flat_cap * 2 : 1;
            while (flat_len + cols > new_cap) new_cap <<= 1;
            flat_vals = (seq_t *)realloc(flat_vals, new_cap * sizeof(seq_t));
            flat_cap  = new_cap;
        }

        if (row == 0) {
            /* First row: a single 1 */
            flat_vals[flat_len++] = (seq_t)1;
        } else {
            /* First element of a new row is always 0 for n > 0 */
            flat_vals[flat_len++] = (seq_t)0;

            /* Index of the first element of the previous row */
            idx_t start_prev = (row - 1) * row / 2;
            seq_t sum = 0;

            /* Compute remaining elements of this row using
             * T(n,k) = T(n,k-1) + T(n-1, n-k). */
            for (idx_t k = 1; k <= row; ++k) {
                seq_t add    = flat_vals[start_prev + (row - k)];
                sum += add;
                flat_vals[flat_len++] = sum;
            }
        }
        ++rows_computed;
    }
}

seq_t
a(idx_t n)
{
    /* Find the row r containing n:
     *   r = floor( (sqrt(8*n+1)-1)/2 )
     *   the triangular number r*(r+1)/2 is the first index of that row. */
    idx_t r = 0;
    while ((r + 1) * (r + 2) / 2 <= n) ++r;

    /* Make sure all needed rows are computed */
    ensure_rows_up_to(r);

    /* Return the requested element */
    return flat_vals[n];
}
