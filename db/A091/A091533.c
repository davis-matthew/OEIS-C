/*
 * A091533  Triangle read by rows, related to Pascal's triangle, starting with
 *          rows 1; 1,1.
 * https://oeis.org/A091533
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, tabl
 * Formula:   a(n) is the sequence obtained by reading the triangle T(n, k)
 *            defined by T(0,0) = 1 T(1,0) = T(1,1) = 1 T(n,k) = T(n-1,k) +
 *            T(n-1,k-1) + T(n-2,k) + T(n-2,k-1) + T(n-2,k-2) for n ≥ 2 , k ≥
 *            0 with all out‑of‑range indices treated as 0. a(n) is the (n‑th)
 *            entry of this triangle when rows are read le...
 * Range:     u8 n <= 29, u16 n <= 82, u32 n <= 283, u64 n <= 964,
 *            u128 n <= 3605, big any n
 * Helpers:   offset_row, row_col_of_n
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <math.h>
#include "../utils/offset_row.h"
#include "../utils/row_col_of_n.h"

static seq_t *triangle_data = NULL;   /* linear storage of all rows seen so far   */
static idx_t   triangle_cap  = 0;     /* allocated number of entries (in idx_t)   */
static idx_t   triangle_rows = 0;     /* highest row index that has been computed */

/* Expand the data array to hold at least rows_needed rows (0‑based) */
static void ensure_rows(idx_t rows_needed)
{
    if (rows_needed <= triangle_rows)
        return;

    /* compute required capacity in terms of number of entries */
    idx_t new_cap = (rows_needed + 1) * (rows_needed + 2) / 2;

    /* realloc if necessary */
    if (new_cap > triangle_cap) {
        seq_t *tmp = (seq_t *)realloc(triangle_data,
                                      (size_t)new_cap * sizeof(seq_t));
        if (!tmp) abort();                /* allocation failure */
        triangle_data = tmp;
        triangle_cap  = new_cap;
    }

    /* initialise new rows */
    for (idx_t r = triangle_rows + 1; r <= rows_needed; ++r) {
        idx_t row_size = r + 1;
        idx_t base = offset_row(r);

        /* compute each entry of row r */
        for (idx_t k = 0; k < row_size; ++k) {
            seq_t val = 0;

            /* contributions from row r-1 */
            idx_t prev_base = offset_row(r - 1);
            if (k < r)                val += triangle_data[prev_base + k];
            if (k > 0)                val += triangle_data[prev_base + k - 1];

            /* contributions from row r-2 (if it exists) */
            if (r >= 2) {
                idx_t prev2_base = offset_row(r - 2);
                if (k < r - 1)        val += triangle_data[prev2_base + k];
                if (k > 0 && k - 1 < r - 1)
                                     val += triangle_data[prev2_base + k - 1];
                if (k > 1)            val += triangle_data[prev2_base + k - 2];
            }

            triangle_data[base + k] = val;
        }
    }
    triangle_rows = rows_needed;
}

/* --- public function ----------------------------------------------------- */

seq_t a(idx_t n)
{
    /* initialise triangle with first two rows if needed */
    if (!triangle_data) {
        idx_t init_cap = 3;                     /* rows 0 and 1 need 3 entries */
        triangle_data = (seq_t *)malloc(init_cap * sizeof(seq_t));
        if (!triangle_data) abort();
        triangle_cap = init_cap;
        triangle_data[0] = 1;                   /* T(0,0) */
        triangle_data[1] = 1;                   /* T(1,0) */
        triangle_data[2] = 1;                   /* T(1,1) */
        triangle_rows = 1;                      /* rows 0 and 1 computed */
    }

    /* find row and column corresponding to index n */
    idx_t r, k;
    row_col_of_n(n, &r, &k);

    /* ensure all required rows are available */
    ensure_rows(r);

    return triangle_data[offset_row(r) + k];
}
