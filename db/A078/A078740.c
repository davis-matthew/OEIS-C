/*
 * A078740  Triangle of generalized Stirling numbers S_{3,2}(n,k) read by rows
 *          (n>=1, 2<=k<=2n).
 * https://oeis.org/A078740
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf, easy
 * Formula:   not stated
 * Range:     u8 n <= 9, u16 n <= 17, u32 n <= 39, u64 n <= 122, u128 n <= 327,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A078740 – Triangle of generalized Stirling numbers S_{3,2}(n,k),
 *  read by rows.  The function a(idx_t n) returns the n-th term in
 *  row-wise order.  The triangle is defined by:
 *      a(1,2) = 1
 *      a(n,k) = (n-1+k)(n-2+k)·a(n-1,k)
 *                + 2(n-2+k)·a(n-1,k-1)
 *                + a(n-1,k-2)              for n ≥ 2, 2 ≤ k ≤ 2n
 *  The mapping from the linear index n to (row nrow,k) is
 *      nrow = ⌊√(n-1)⌋ + 1
 *      k    = n - (nrow-1)² + 1          (k runs from 2 to 2·nrow)
 *
 *  The implementation builds the triangle lazily as required and
 *  stores it in a static 2‑D array allocated with malloc/calloc.
 */

#include <stdlib.h>
#include <math.h>
#include <stdint.h>
#include <stdbool.h>

/* External integer types provided by the build */
extern seq_t binomial(idx_t n, idx_t k);  /* unused, but allowed */

static seq_t **triangle = NULL;   /* array of row pointers */
static idx_t rows_allocated = 0;  /* capacity of triangle pointer array */
static idx_t max_row_computed = 0;/* number of rows actually computed */

/* Ensure that rows 1..need_row are computed and stored in triangle */
static void ensure_rows(idx_t need_row)
{
    if (need_row <= max_row_computed)
        return;

    /* enlarge the triangle pointer array if necessary */
    if (need_row >= rows_allocated) {
        size_t new_cap = rows_allocated ? rows_allocated * 2 : 8;
        while (new_cap <= need_row)
            new_cap <<= 1;
        triangle = (seq_t **)realloc(triangle, new_cap * sizeof(seq_t *));
        rows_allocated = (idx_t)new_cap;
    }

    /* compute missing rows */
    for (idx_t r = max_row_computed + 1; r <= need_row; ++r) {
        size_t len = 2 * r + 1;            /* indices 0 .. 2r */
        seq_t *row = (seq_t *)calloc(len, sizeof(seq_t));
        if (!row) abort();                /* allocation failure */

        if (r == 1) {
            row[2] = (seq_t)1;
        } else {
            /* previous row data */
            seq_t *prev = triangle[r - 1];
            idx_t prev_len = 2 * (r - 1);

            for (idx_t k = 2; k <= 2 * r; ++k) {
                seq_t t1 = 0, t2 = 0, t3 = 0;
                if (k <= prev_len) {
                    seq_t mul = (seq_t)(r - 1 + k) * (seq_t)(r - 2 + k);
                    t1 = mul * prev[k];
                }
                if (k - 1 <= prev_len) {
                    seq_t mul = (seq_t)2 * (seq_t)(r - 2 + k);
                    t2 = mul * prev[k - 1];
                }
                if (k - 2 <= prev_len) {
                    t3 = prev[k - 2];
                }
                row[k] = t1 + t2 + t3;
            }
        }
        triangle[r] = row;
    }
    max_row_computed = need_row;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;   /* convention for undefined indices */

    /* determine row and column in the triangle */
    idx_t row = (idx_t)floor(sqrt((long double)(n - 1))) + 1;
    idx_t prev_end = (row - 1) * (row - 1);
    idx_t kpos = n - prev_end;          /* 1 .. (2row-1) */
    idx_t k = kpos + 1;                 /* 2 .. 2row */

    ensure_rows(row);
    return triangle[row][k];
}
