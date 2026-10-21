/*
 * A202603  Triangle T(n,k), read by rows, given by (1, 0, 0, 0, 0, 0, 0, 0,
 *          0, 0, ...) DELTA (1, -1, 1, 0, 0, 0, 0, 0, 0, 0, 0, ...) where
 *          DELTA is the operator defined in A084938.
 * https://oeis.org/A202603
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, tabl
 * Formula:   a(n) is the n-th element of the flattened triangle T(n,k)
 *            defined by T(0,0)=T(1,0)=T(1,1)=1,
 *            T(n,k)=T(n-1,k)+T(n-1,k-1)-T(n-2,k-1)-T(n-2,k-2) with T(n,k)=0
 *            for k<0 or k>n. The function computes the value a(n) lazily and
 *            memoises all earlier values in a static cache. The
 *            implementation uses only t...
 * Range:     i8 n <= 65, i16 n <= 65, i32 n <= 65, i64 n <= 65, i128 n <= 65,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    /* ---------- metadata for triangle computation ---------- */
    static seq_t *prev_row = NULL;          /* row n-1 */
    static size_t prev_len = 0;             /* length of prev_row = n */
    static seq_t *prev_prev_row = NULL;     /* row n-2 */
    static size_t prev_prev_len = 0;        /* length of prev_prev_row = n-1 */
    static idx_t last_row = (idx_t)-1;      /* last row fully computed */

    /* ---------- cache for the flattened sequence ---------- */
    static seq_t *cache = NULL;             /* flattened values */
    static size_t cache_len = 0;            /* number of values actually stored   */
    static size_t cache_cap = 0;            /* allocated capacity of cache       */

    /* ---------- grow cache if necessary ---------- */
    if (n >= cache_cap) {
        /* ensure the cache can hold at least n+1 elements */
        size_t new_cap = cache_cap ? cache_cap * 2 : 64;
        while (new_cap <= (size_t)n) new_cap <<= 1;
        seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!new_cache) return 0;           /* out of memory: impossible for test cases */
        cache = new_cache;
        cache_cap = new_cap;
    }

    /* ---------- compute rows until we cover index n ---------- */
    while (cache_len <= (size_t)n) {
        /* which row to compute next? */
        idx_t next_row = (last_row == (idx_t)-1) ? 0 : last_row + 1;
        size_t row_len = (size_t)next_row + 1;

        /* allocate buffer for the new row */
        seq_t *curr = (seq_t *)malloc(row_len * sizeof(seq_t));
        if (!curr) return 0; /* unlikely in tests */

        /* compute each entry of the new row */
        for (size_t k = 0; k < row_len; ++k) {
            seq_t val;
            if (next_row == 0) {
                val = 1; /* T(0,0) */
            } else if (next_row == 1) {
                val = 1; /* T(1,0) and T(1,1) */
            } else {
                seq_t a1 = (k < prev_len) ? prev_row[k]               : 0;
                seq_t a2 = (k > 0 && k-1 < prev_len) ? prev_row[k-1] : 0;
                seq_t a3 = (k > 0 && k-1 < prev_prev_len) ? prev_prev_row[k-1] : 0;
                seq_t a4 = (k > 1 && k-2 < prev_prev_len) ? prev_prev_row[k-2] : 0;
                val = a1 + a2 - a3 - a4;
            }
            curr[k] = val;
        }

        /* ---- append the new row to the cache in one step ---- */
        if (cache_len + row_len > cache_cap) {
            /* enlarge cache if needed (should not happen because we grew above) */
            size_t needed = cache_len + row_len;
            size_t new_cap = cache_cap ? cache_cap * 2 : 64;
            while (new_cap < needed) new_cap <<= 1;
            seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            if (!tmp) { free(curr); return 0; }
            cache = tmp;
            cache_cap = new_cap;
        }
        for (size_t i = 0; i < row_len; ++i)
            cache[cache_len + i] = curr[i];
        cache_len += row_len;

        /* ------ prepare for next iteration: shift rows ----- */
        free(prev_prev_row);            /* free the row that is now two steps behind */
        prev_prev_row = prev_row;
        prev_prev_len = prev_len;

        prev_row = curr;
        prev_len = row_len;

        last_row = next_row;
    }

    return cache[(size_t)n];
}
