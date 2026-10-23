/*
 * A049424  Triangle read by rows, the Bell transform of n!*binomial(4,n)
 *          (without column 0).
 * https://oeis.org/A049424
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 11, u16 n <= 30, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   ensure_size_t_cap, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Formula: 
 *   a(n, m) = (5*m - n + 1)*a(n-1, m) + a(n-1, m-1),  n ≥ m ≥ 1
 *   a(n, 0) = 0, a(n, m) = 0 for n < m,  a(1,1)=1
 *   a(n) = flattening of the triangle by rows (1‑based offset).
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/seq_realloc.h"   /* seq_realloc(seq_t*,size_t,size_t) */
#include "../utils/ensure_size_t_cap.h"

static seq_t *flat          = NULL;   /* flattened triangle values */
static size_t flat_cap      = 0;
static size_t flat_len      = 0;

static size_t *row_start    = NULL;   /* start index of each row in flat (1‑based) */
static size_t row_start_cap = 0;
static size_t rows_done     = 0;      /* number of rows already computed */

/* enlarge a seq_t array to at least 'need' elements */
static void ensure_seq_cap(seq_t **arr, size_t *cap, size_t need)
{
    if (need <= *cap) return;
    size_t new_cap = (*cap > 0) ? (*cap << 1) : 8;
    if (new_cap < need) new_cap = need;
    *arr = seq_realloc(*arr, *cap, new_cap);
    if (*arr == NULL) abort();
    *cap = new_cap;
}

seq_t a(idx_t n)
{
    /* n is 1‑based; convert to 0‑based index in flattened array */
    size_t idx = (size_t)(n - 1);

    /* build rows until the requested index is available */
    while (flat_len <= idx) {
        /* start a new row */
        ++rows_done; /* now rows_done == current row number */

        /* record start of this row */
        ensure_size_t_cap(&row_start, &row_start_cap, rows_done + 1);
        row_start[rows_done] = flat_len;

        /* compute the entries of this row */
        for (size_t m = 1; m <= rows_done; ++m) {
            seq_t prev_m   = 0;    /* a(rows_done-1, m)   */
            seq_t prev_m_1 = 0;    /* a(rows_done-1, m-1) */

            if (rows_done > 1) {
                if (m <= rows_done - 1) {
                    prev_m = flat[row_start[rows_done - 1] + (m - 1)];
                }
                if (m > 1) {
                    prev_m_1 = flat[row_start[rows_done - 1] + (m - 2)];
                }
            }

            seq_t val;
            if (rows_done == 1 && m == 1) {
                val = 1; /* a(1,1) = 1 */
            } else {
                /* coefficient may become zero or negative for m=1, n≥6 */
                int64_t coeff = (int64_t)(5 * (int64_t)m - (int64_t)rows_done + 1);
                seq_t term1   = 0;
                if (coeff > 0) {
                    term1 = ((seq_t)coeff) * prev_m;
                }
                val = term1 + prev_m_1;
            }

            /* store the value */
            ensure_seq_cap(&flat, &flat_cap, flat_len + 1);
            flat[flat_len++] = val;
        }
    }

    return flat[idx];
}
