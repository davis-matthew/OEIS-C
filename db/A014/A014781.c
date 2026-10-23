/*
 * A014781  Seidel's triangle, read by rows.
 * https://oeis.org/A014781
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  tabf, nonn
 * Formula:   Seidel's triangle read by rows T(1,1)=1; For n>1 and 1≤k≤⌈n/2⌉:
 *            T(n,k)=T(n,k+(-1)^n)+T(n-1,k), with T(n,k)=0 when k is out of
 *            range. a(n) is the n‑th entry of the concatenation of all rows
 * Range:     u8 n <= 25, u16 n <= 48, u32 n <= 48, u64 n <= 48, u128 n <= 48,
 *            big any n
 * Helpers:   alloc_seq, free_seq, grow_seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>
#include "../utils/alloc_seq.h"
#include "../utils/free_seq.h"
#include "../utils/grow_seq_realloc.h"

#ifdef __cplusplus
/* ---------- C++ allocation helpers ---------- */

#else
/* ---------- C allocation helpers ---------- */

#endif

seq_t a(idx_t n)
{
    /* static cache of already produced values */
    static seq_t* cache     = NULL;
    static size_t cache_size  = 0, cache_cap   = 0;

    /* static storage for the previous row */
    static seq_t* prev          = NULL;
    static size_t prev_len      = 0;

    /* how many rows of the triangle have been produced */
    static idx_t rows_generated = 0;

    /* first call: initialise the first row */
    if (cache == NULL) {
        cache_cap  = 64;
        cache      = alloc_seq(cache_cap);
        prev_len   = 1;
        prev       = alloc_seq(prev_len);
        prev[0]    = (seq_t)1;
        cache[0]   = prev[0];
        cache_size = 1;
        rows_generated = 1;
    }

    /* generate new rows until we have at least n entries in the cache */
    while (cache_size < (size_t)n) {
        idx_t r = ++rows_generated;                   /* current row number (2,3,…) */
        size_t new_len = ((size_t)r + 1) / 2;           /* ceil(r/2) */

        seq_t* new_arr = alloc_seq(new_len);

        if (r & 1) { /* odd row: compute left‑to‑right */
            for (size_t idx = 0; idx < new_len; ++idx) {
                seq_t left = (idx == 0) ? (seq_t)0 : new_arr[idx-1];
                seq_t up   = (idx < prev_len) ? prev[idx] : (seq_t)0;
                new_arr[idx] = left + up;
            }
        } else { /* even row: compute right‑to‑left */
            for (size_t rev = 0; rev < new_len; ++rev) {
                size_t idx = new_len - 1 - rev;
                seq_t right = (idx == new_len - 1) ? (seq_t)0 : new_arr[idx+1];
                seq_t up    = (idx < prev_len) ? prev[idx] : (seq_t)0;
                new_arr[idx] = right + up;
            }
        }

        /* append the new row to the cache */
        if (cache_size + new_len > cache_cap) {
            size_t new_cap = cache_cap ? cache_cap * 2 : 64;
            while (new_cap < cache_size + new_len) new_cap *= 2;
            cache = grow_seq_realloc(cache, cache_cap, new_cap);
            cache_cap = new_cap;
        }
        for (size_t idx = 0; idx < new_len; ++idx)
            cache[cache_size++] = new_arr[idx];

        /* free the old previous row and adopt the new row */
        free_seq(prev);
        prev     = new_arr;
        prev_len = new_len;
    }

    return cache[(size_t)n - 1];
}
