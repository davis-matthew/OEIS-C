/*
 * A162741  Fibonacci-Pascal triangle; same as Pascal triangle, but beginning
 *          another Pascal triangle to the right of each row starting at row
 *          2.
 * https://oeis.org/A162741
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   a(n) is the n-th element of the flattened Fibonacci‑Pascal *
 *            triangle. Row r has length 2*r-1, first entry 1, last two
 *            entries 1, * and interior entries defined by
 *            T(r,k)=T(r-1,k-1)+T(r-1,k). The * sequence is generated row by
 *            row; the n-th element (1‑based) is * returned
 * Range:     u8 n <= 105, u16 n <= 333, u32 n <= 1240, u64 n <= 4521,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static void grow_array(seq_t **arr, size_t *cap, size_t needed)
{
    if (*cap < needed) {
        size_t new_cap = (*cap == 0) ? 1 : (*cap << 1);
        while (new_cap < needed) new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(*arr, new_cap * sizeof(seq_t));
        *arr = tmp;
        *cap = new_cap;
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return 0; /* undefined, but harmless */

    /* static cache of flattened sequence values */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;
    static size_t cache_len = 0;          /* number of cached terms */

    /* two buffers for successive rows */
    static seq_t *row[2] = {NULL, NULL};
    static size_t row_cap[2] = {0, 0};
    static size_t row_len[2] = {0, 0};     /* current valid length of each buffer */
    static idx_t cur_buf = 0;             /* index of buffer holding last built row */
    static idx_t rows_built = 0;           /* number of rows already constructed */

    /* target index in 0‑based form */
    idx_t target = n - 1;

    /* Build rows until the requested index is cached */
    while (cache_len <= (size_t)target) {
        idx_t next_row = rows_built + 1;             /* row number (1‑based) */
        size_t len = (size_t)(2 * next_row - 1);     /* length of this row */

        /* ensure buffers are large enough */
        grow_array(&row[!cur_buf], &row_cap[!cur_buf], len);

        seq_t *prev = row[cur_buf];
        seq_t *curr = row[!cur_buf];
        size_t prev_len = row_len[cur_buf];          /* length of previous row */

        /* build current row */
        curr[0] = 1;
        if (len > 1) {
            curr[len - 1] = 1;
            curr[len - 2] = 1;
        }
        if (len > 3) {                               /* interior entries */
            for (size_t k = 1; k < len - 2; ++k) {
                seq_t left  = 0;
                seq_t right = 0;
                if (k <= prev_len) left  = prev[k - 1];
                if (k < prev_len)  right = prev[k];
                curr[k] = left + right;
            }
        }

        /* append new row to cache */
        grow_array(&cache, &cache_cap, cache_len + len);
        for (size_t i = 0; i < len; ++i)
            cache[cache_len + i] = curr[i];
        cache_len += len;

        /* prepare for next iteration */
        row_len[!cur_buf] = len;  /* store length of the row we just built */
        cur_buf ^= 1;             /* swap current row buffer */
        rows_built = next_row;
    }

    return cache[target];
}
