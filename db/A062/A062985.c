/*
 * A062985  Generalized Catalan array FS(5; n,r).
 * https://oeis.org/A062985
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, tabf
 * Formula:   not stated
 * Range:     u8 n <= 39, u16 n <= 56, u32 n <= 56, u64 n <= 56, u128 n <= 56,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Formula: a(n) is the flattened array FS(5; n,r) defined by
 *   a(0,0) = 1
 *   a(n, -1) = 0   (n >= 1)
 *   a(n, r) = 0    if r > 4*n
 *   otherwise     a(n,r) = a(n, r-1) + a(n-1, r)
 *
 * The array is read row‑wise, so a(k) is a(n,r) for the unique pair
 * (n,r) with k = (n+1)(2n+1) - (4n - r).
 *
 * This function computes any term a(n) on demand, caching all terms
 * up to the largest index requested.  Each term is computed exactly
 * once, and earlier terms are reused for later requests.  The algorithm
 * runs in O(n) time for a single call up to index n and uses O(n)
 * storage.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    /* static cache state */
    static seq_t *cache = NULL;          /* array of computed terms   */
    static size_t cache_len = 0;         /* current number of terms  */
    static size_t cache_cap = 0;         /* allocated capacity       */
    static idx_t next_row = 0;           /* next row to expand       */
    static idx_t next_row_start = 0;     /* global index where next_row begins */

    /* init first row */
    if (cache_len == 0) {
        cache_cap = 8;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        cache[0] = 1;        /* a(0,0) */
        cache_len = 1;
        next_row = 1;
        next_row_start = 1;
    }

    /* expand cache until index n is available */
    while (cache_len <= (size_t)n) {
        /* length of the new row */
        size_t row_len = 4 * next_row + 1;
        /* previous row length and start index */
        size_t prev_len = next_row == 0 ? 0 : 4 * (next_row - 1) + 1;
        idx_t  prev_start = next_row_start - (prev_len);

        /* ensure enough capacity */
        if (cache_len + row_len > cache_cap) {
            size_t new_cap = cache_cap > 0 ? cache_cap * 2 : 8;
            while (new_cap < cache_len + row_len) new_cap <<= 1;
            seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            cache = tmp;
            cache_cap = new_cap;
        }

        /* compute the row */
        for (size_t col = 0; col < row_len; ++col) {
            idx_t   global_idx = next_row_start + col;
            size_t  id          = (size_t)global_idx;   /* array index */

            if (col == 0) {                             /* first column */
                if (next_row == 0) {
                    cache[id] = 1;                       /* a(0,0) */
                } else {
                    cache[id] = cache[(size_t)prev_start];
                }
            } else {
                seq_t prev_in_row = cache[id - 1];
                seq_t prev_row_val = 0;
                if (col < prev_len) {
                    prev_row_val = cache[(size_t)prev_start + col];
                }
                cache[id] = prev_in_row + prev_row_val;
            }
        }

        /* advance to next row */
        cache_len += row_len;
        next_row_start += (idx_t)row_len;
        ++next_row;
    }

    return cache[(size_t)n];
}
