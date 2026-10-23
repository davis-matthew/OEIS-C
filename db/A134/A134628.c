/*
 * A134628  Sum-fill array starting with (2,1).
 * https://oeis.org/A134628
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   not stated
 * Range:     u8 n <= 85, u16 n <= 85, u32 n <= 85, u64 n <= 85, u128 n <= 85,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sum‑fill array starting with (2,1) (rows concatenated) */
#include <stdlib.h>
#include <stdbool.h>

/* seq_t and idx_t are supplied by the build system. */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* out of range, return 0 */

    /* static state for memoisation */
    static seq_t *cache   = NULL;   /* concatenated sequence values   */
    static size_t cache_len = 0;    /* number of terms stored          */
    static size_t cache_cap = 0;    /* allocated capacity of cache     */
    static seq_t *row      = NULL;  /* current row                     */
    static size_t row_len  = 0;     /* length of current row            */
    static bool init = false;      /* whether the state is initialised */

    /* initialise on first call */
    if (!init) {
        /* row 1 = (2,1) */
        row_len = 2;
        row = (seq_t *)malloc(row_len * sizeof(seq_t));
        row[0] = (seq_t)2;
        row[1] = (seq_t)1;

        /* cache starts with the first row */
        cache_cap = 8;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        cache[0] = (seq_t)2;
        cache[1] = (seq_t)1;
        cache_len = 2;

        init = true;
    }

    /* generate rows until the cache contains at least n terms */
    while (cache_len < (size_t)n) {
        /* 1. sum‑fill the current row */
        size_t temp_len = 2 * row_len - 1;
        seq_t *temp = (seq_t *)malloc(temp_len * sizeof(seq_t));

        size_t t = 0;
        for (size_t i = 0; i < row_len; ++i) {
            temp[t++] = row[i];
            if (i + 1 < row_len)
                temp[t++] = row[i] + row[i + 1];
        }

        /* 2. deduplicate within the new row (first occurrence only) */
        seq_t *new_row = (seq_t *)malloc(temp_len * sizeof(seq_t));
        size_t new_len = 0;
        for (size_t i = 0; i < temp_len; ++i) {
            seq_t v = temp[i];
            bool seen = false;
            for (size_t j = 0; j < new_len; ++j) {
                if (new_row[j] == v) { seen = true; break; }
            }
            if (!seen) new_row[new_len++] = v;
        }
        free(temp);

        /* 3. append new row to cache */
        if (cache_cap < cache_len + new_len) {
            size_t new_cap = cache_cap ? cache_cap * 2 : 16;
            while (new_cap < cache_len + new_len) new_cap *= 2;
            seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            cache = tmp;
            cache_cap = new_cap;
        }
        for (size_t i = 0; i < new_len; ++i) cache[cache_len++] = new_row[i];

        /* 4. prepare for next iteration */
        free(row);
        row = new_row;
        row_len = new_len;
    }

    return cache[n - 1];
}
