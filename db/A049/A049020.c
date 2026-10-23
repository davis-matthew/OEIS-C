/*
 * A049020  Triangle of numbers a(n,k), 0 <= k <= n: number of set partitions
 *          of {1,2,...,n} in which exactly k of the blocks have been
 *          distinguished.
 * https://oeis.org/A049020
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl, nice, easy
 * Formula:   a(n) = a(r,k) where r is the unique integer such that r*(r+1)/2
 *            <= n < (r+1)*(r+2)/2, k = n - r*(r+1)/2. The array a(r,k) is
 *            defined by a(0,0)=1 and for r>0 a(r,k) = a(r-1,k-1) +
 *            (k+1)*a(r-1,k) + (k+1)*a(r-1,k+1), where terms with out‑of‑range
 *            indices are treated as 0
 * Range:     u8 n <= 21, u16 n <= 45, u32 n <= 107, u64 n <= 302,
 *            u128 n <= 824, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Essential headers */
#include <stdlib.h>

/* Global static storage for the triangle */
static seq_t **triangle = NULL;   /* array of rows; triangle[i] points to row i */
static idx_t  rows_allocated = 0; /* number of rows currently allocated */

/* Helper: ensure the triangle has at least rows rows_needed calculated */
static void ensure_rows(idx_t rows_needed)
{
    if (rows_allocated > rows_needed) return;   /* already enough */

    /* Grow triangle array to hold the required number of rows */
    if (triangle == NULL) {
        triangle = (seq_t **)calloc(rows_needed + 1, sizeof(seq_t *));
        rows_allocated = 0;
    }
    else {
        triangle = (seq_t **)realloc(triangle, (rows_needed + 1) * sizeof(seq_t *));
    }

    /* Compute new rows */
    while (rows_allocated <= rows_needed) {
        idx_t new_row = rows_allocated;

        /* Allocate space for this row: new_row + 1 elements */
        seq_t *row = (seq_t *)malloc(sizeof(seq_t) * (new_row + 1));

        if (new_row == 0) {
            /* Base case: a(0,0) = 1 */
            row[0] = (seq_t)1;
        } else {
            /* Use previous row for recurrence */
            seq_t *prev = triangle[new_row - 1];
            for (idx_t k = 0; k <= new_row; ++k) {
                seq_t val = 0;

                if (k > 0)                     /* a(n-1,k-1) */
                    val += prev[k - 1];

                if (k <= new_row - 1) {         /* (k+1)*a(n-1,k) */
                    seq_t term = prev[k];
                    term *= (seq_t)(k + 1);
                    val += term;
                }

                if (k + 1 <= new_row - 1) {     /* (k+1)*a(n-1,k+1) */
                    seq_t term = prev[k + 1];
                    term *= (seq_t)(k + 1);
                    val += term;
                }

                row[k] = val;
            }
        }

        triangle[new_row] = row;
        rows_allocated++;
    }
}

seq_t a(idx_t n)
{
    /* Find row r and column k such that the (r,k) entry is the nth element
       in the row‑ordered enumeration of the triangle. */
    idx_t r = 0;
    idx_t cum = 0;          /* cumulative number of entries up to row r-1 */

    while (cum + (r + 1) <= n) {
        cum += r + 1;
        r++;
    }

    idx_t k = n - cum;      /* column index within the row */

    /* Ensure the triangle is computed up to row r */
    ensure_rows(r);

    return triangle[r][k];
}
