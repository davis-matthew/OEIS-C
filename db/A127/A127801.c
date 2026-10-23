/*
 * A127801  Inverse of number triangle A(n,k) = 1/(3 - 2*0^n) if k <= n <= 2k,
 *          0 otherwise.
 * https://oeis.org/A127801
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, tabl
 * Formula:   a(n) is the nth element (0‑indexed) obtained by row‑wise
 *            flattening the inverse of the triangle A(n,k)=1/(3-2*0^n) when
 *            k≤n≤2k, otherwise 0
 * Range:     i8 n <= 71, i16 n <= 71, i32 n <= 71, i64 n <= 71, i128 n <= 71,
 *            big any n
 * Helpers:   offset_row
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/offset_row.h"

static seq_t *a_cache = NULL;       /* flattened sequence cache   */
static size_t a_cache_cap = 0;      /* capacity of a_cache        */
static idx_t   a_cache_len = 0;     /* number of elements cached  */

static int16_t *B_data     = NULL;  /* forward star triangle     */
static idx_t   B_rows      = 0;      /* number of rows stored     */
static int32_t *active_sum = NULL;  /* running sums for columns  */
static idx_t   active_sum_len = 0;   /* size of active_sum array  */

/* Ensure a_cache has room for at least 'size' elements. */
static void ensure_a_cache_size(idx_t size)
{
    if (size <= a_cache_cap) return;
    size_t new_cap = a_cache_cap ? a_cache_cap * 2 : 16;
    if (new_cap < size) new_cap = size;
    a_cache = (seq_t *)realloc(a_cache, new_cap * sizeof(seq_t));
    a_cache_cap = new_cap;
}

/* Ensure storage for 'rows' rows of B and active_sum. */
static void ensure_B_and_active(idx_t rows)
{
    if (rows <= B_rows) return;
    /* resize B_data to accommodate rows rows (0‑based count) */
    idx_t total = rows * (rows + 1) / 2;
    B_data = (int16_t *)realloc(B_data, total * sizeof(int16_t));

    /* resize active_sum to accommodate columns up to 'rows' */
    active_sum_len = rows;
    active_sum = (int32_t *)realloc(active_sum, active_sum_len * sizeof(int32_t));

    /* zero new active_sum entries (they were never used yet) */
    for (idx_t i = B_rows; i < rows; ++i) active_sum[i] = 0;

    B_rows = rows;
}

/* Compute the next row (row index = B_rows) of the inverse triangle.  */
static void compute_next_row(void)
{
    idx_t r = B_rows;            /* index of row to compute                     */
    ensure_B_and_active(r + 1);  /* allocate if necessary                         */

    if (r == 0) {                /* the first row is trivial                     */
        B_data[0] = 1;
        ensure_a_cache_size(a_cache_len + 1);
        a_cache[a_cache_len++] = (seq_t)1;
        return;
    }

    /* Prepare indices for the active‑sum update.                      */
    idx_t low_prev = r / 2;                /* ceil((r-1)/2)                               */
    idx_t low_new  = (r + 1) / 2;          /* ceil(r/2)                                    */
    idx_t start_prev = offset_row(r - 1);  /* first index of row r-1                       */

    /* Add the (r-1)th row to the active sums.                      */
    for (idx_t k = 0; k < r; ++k) {
        active_sum[k] += B_data[start_prev + k];
    }

    /* If the lower bound increased, subtract the row that fell out. */
    if (low_new > low_prev) {
        idx_t rem = low_prev;
        idx_t start_rem = offset_row(rem);
        for (idx_t k = 0; k <= rem; ++k) {
            active_sum[k] -= B_data[start_rem + k];
        }
    }

    /* Compute the current row from the active sums.                  */
    idx_t start = offset_row(r);
    for (idx_t k = 0; k < r; ++k) {
        int16_t val = -(int16_t)active_sum[k];
        B_data[start + k] = val;

        ensure_a_cache_size(a_cache_len + 1);
        a_cache[a_cache_len++] = (seq_t)val;
    }
    int16_t last_val = 3;
    B_data[start + r] = last_val;

    ensure_a_cache_size(a_cache_len + 1);
    a_cache[a_cache_len++] = (seq_t)last_val;
}

/* Public function required by the problem statement.                 */
seq_t a(idx_t n)
{
    while (a_cache_len <= n) {
        compute_next_row();
    }
    return a_cache[n];
}
