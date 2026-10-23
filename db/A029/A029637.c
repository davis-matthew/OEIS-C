/*
 * A029637  Numbers in the (1,2)-Pascal triangle A029635 that are different
 *          from 2.
 * https://oeis.org/A029637
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   Numbers in the (1,2)-Pascal triangle A029635 that are different
 *            from 2. The triangle T(r,k) (rows r ≥ 0, 0 ≤ k ≤ r) is defined
 *            by: T(0,0) = 1 T(r,0) = 1 (left edge) T(r,r) = T(r-1,r-1) + 2
 *            (right edge) T(r,k) = T(r-1,k-1) + T(r-1,k) (interior, 0 < k <
 *            r) The sequence a(n) lists the triangle entri...
 * Range:     u8 n <= 49, u16 n <= 71, u32 n <= 71, u64 n <= 71, u128 n <= 71,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* static cache: all previously computed entries */
    static seq_t *data = NULL;          /* flattened triangle entries */
    static size_t data_capacity = 0;
    static size_t data_size = 0;        /* number of entries stored */

    /* static cache for the most recently computed row */
    static seq_t *prev_row = NULL;      /* previous row array */
    static idx_t  prev_len = 0;         /* length of previous row */
    static idx_t  cached_rows = 0;      /* number of rows already built */

    /* ------------------------------------------------------------------
     *  Find the row needed for index n.
     * ------------------------------------------------------------------*/
    idx_t need_row;
    {
        idx_t low = 0, high = 1;
        /* find an upper bound for the row number */
        while ( ((unsigned __int128)high * (high + 1)) / 2 < n ) high <<= 1;
        /* binary search lower bound */
        while (low < high) {
            idx_t mid = low + (high - low) / 2;
            if ( ((unsigned __int128)mid * (mid + 1)) / 2 >= n )
                high = mid;
            else
                low = mid + 1;
        }
        need_row = low; /* rows are 1‑based (row 1 has length 1) */
    }

    /* ------------------------------------------------------------------
     *  Extend cache up to the required row if necessary.
     * ------------------------------------------------------------------*/
    if (cached_rows < need_row) {
        for (idx_t r = cached_rows + 1; r <= need_row; ++r) {
            idx_t len = r;                     /* length of this row */
            seq_t *cur = (seq_t *)malloc(len * sizeof(seq_t));
            for (idx_t k = 0; k < len; ++k) {
                seq_t val;
                if (r == 1) {            /* first row */
                    val = 1;
                } else if (k == 0) {     /* left edge */
                    val = 1;
                } else if (k == len - 1) {  /* right edge */
                    val = prev_row[prev_len - 1] + 2;
                } else {                /* interior */
                    val = prev_row[k - 1] + prev_row[k];
                }
                cur[k] = val;

                /* append to flattened data array */
                if (data_size == data_capacity) {
                    size_t new_cap = data_capacity ? data_capacity * 2 : 16;
                    seq_t *tmp = (seq_t *)realloc(data, new_cap * sizeof(seq_t));
                    data = tmp;
                    data_capacity = new_cap;
                }
                data[data_size++] = val;
            }
            /* free previous row and store current row as previous */
            free(prev_row);
            prev_row = cur;
            prev_len = len;
        }
        cached_rows = need_row;
    }

    /* ------------------------------------------------------------------
     *  Retrieve the required element.
     * ------------------------------------------------------------------*/
    /* total entries in rows before the needed row */
    idx_t prev_total =
        ( (unsigned __int128)(need_row - 1) * need_row ) / 2;
    /* position within the current row (0‑based) */
    idx_t offset = n - prev_total - 1;
    return data[prev_total + offset];
}
