/*
 * A128100  Triangle read by rows: T(n,k) is the number of ways to tile a 2 X
 *          n rectangle with k pieces of 2 X 2 tiles and n-2k pieces of 1 X 2
 *          tiles (0 <= k <= floor(n/2)).
 * https://oeis.org/A128100
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   not stated
 * Range:     u8 n <= 36, u16 n <= 65, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n) is the row‑major sequence obtained from the triangle T(n,k)
 * where
 *   T(n,k) = T(n-1,k) + T(n-2,k) + T(n-2,k-1),
 *   T(0,0)=T(1,0)=1.
 * a(n) is the element of T at the flat index n. */
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static storage for the whole triangle in row‑major order. */
    static seq_t *data     = NULL;
    static size_t data_len = 0;
    static size_t data_cap = 0;

    static idx_t *row_start = NULL;
    static size_t row_cap   = 0;
    static idx_t  row_count = 0;    /* number of rows already generated */

    /* one‑time allocation and initialization of rows 0 and 1 */
    if (!data) {
        data_cap   = 16;
        data       = (seq_t *)malloc(data_cap * sizeof(seq_t));
        row_cap    = 16;
        row_start  = (idx_t *)malloc(row_cap * sizeof(idx_t));

        /* row 0 */
        data[0]      = 1;
        row_start[0] = 0;

        /* row 1 */
        data[1]      = 1;
        row_start[1] = 1;

        data_len  = 2;
        row_count = 2;
    }

    /* generate rows until the desired row is available */
    while (row_start[row_count - 1] <= n) {
        idx_t  new_row = row_count;            /* next row to compute   */
        idx_t  len     = new_row / 2 + 1;      /* number of cells in row */
        idx_t  start   = data_len;             /* start index of new row */

        /* make data large enough */
        if (data_len + len > data_cap) {
            size_t new_cap = data_cap ? data_cap : 16;
            while (new_cap < data_len + len) new_cap <<= 1;
            seq_t *tmp = (seq_t *)realloc(data, new_cap * sizeof(seq_t));
            if (!tmp) abort();
            data     = tmp;
            data_cap = new_cap;
        }

        /* make row_start large enough */
        if (row_count + 1 > row_cap) {
            size_t new_cap = row_cap ? row_cap : 16;
            while (new_cap < row_count + 1) new_cap <<= 1;
            idx_t *tmp = (idx_t *)realloc(row_start, new_cap * sizeof(idx_t));
            if (!tmp) abort();
            row_start = tmp;
            row_cap   = new_cap;
        }

        /* compute each entry of the new row */
        for (idx_t k = 0; k < len; ++k) {
            seq_t val = 0;

            /* T(new_row-1, k) */
            idx_t prev1 = new_row - 1;
            if (k <= prev1 / 2) {
                val += data[row_start[prev1] + k];
            }

            /* T(new_row-2, k) and T(new_row-2, k-1) */
            if (new_row >= 2) {
                idx_t prev2 = new_row - 2;
                if (k <= prev2 / 2) {
                    val += data[row_start[prev2] + k];
                }
                if (k > 0 && (k - 1) <= prev2 / 2) {
                    val += data[row_start[prev2] + (k - 1)];
                }
            }

            data[start + k] = val;
        }

        data_len          += len;
        row_start[new_row] = start;
        ++row_count;
    }

    /* locate row and column for flat index n */
    idx_t r = 0;
    while (row_start[r + 1] <= n) ++r;

    idx_t k = n - row_start[r];

    return data[row_start[r] + k];
}
