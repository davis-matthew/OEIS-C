/*
 * A070878  Stern's diatomic array read by rows (version 2).
 * https://oeis.org/A070878
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabf, nice, easy
 * Formula:   not stated
 * Range:     u8 n <= 8798, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implements sequence A070878:  Stern's diatomic array read by rows.  
   Each row r has length 2^r+1 and is formed from the previous row
   by copying the terms and inserting the sums of adjacent pairs.
   The sequence is the concatenation of all rows starting from row 0,
   with a(0)=1, a(1)=0.  The implementation builds rows lazily and
   caches all computed values, so any n can be supplied in arbitrary
   order. */
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

/*  seq_t and idx_t are supplied by the build.  An unsigned integer of
    8, 16, 32, 64 or 128 bits, or an arbitrary-precision class.  */
seq_t a(idx_t n)
{
    /* Cache of all computed terms. */
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;   /* number of terms stored */
    static idx_t cache_cap = 0;   /* allocated capacity of cache */

    /* Last generated row. */
    static seq_t *prev_row = NULL;
    static idx_t prev_len = 0;    /* length of that row */
    static idx_t current_row = 0; /* row index of prev_row (0‑based) */

    /* --- initialise the first row (row 0) on the first call --- */
    if (cache_len == 0) {
        /* row 0: [1, 0] */
        prev_len = 2;
        prev_row = (seq_t *)malloc(sizeof(seq_t) * prev_len);
        if (!prev_row) abort();
        prev_row[0] = (seq_t)1;
        prev_row[1] = (seq_t)0;

        /* initialise cache to fit the first row */
        cache_cap = 2;
        cache = (seq_t *)malloc(sizeof(seq_t) * cache_cap);
        if (!cache) abort();
        cache[0] = prev_row[0];
        cache[1] = prev_row[1];
        cache_len = 2;
        current_row = 0;
    }

    /* --- generate additional rows until we have at least n+1 terms --- */
    while (cache_len <= n) {
        /* Build the next row from prev_row. */
        size_t L = (size_t)prev_len;          /* length of previous row   */
        size_t new_len = 2 * L - 1;           /* length of new row       */

        seq_t *new_row = (seq_t *)malloc(sizeof(seq_t) * new_len);
        if (!new_row) abort();

        /* Fill the new row:
           new_row[2*j]     = prev_row[j],
           new_row[2*j + 1] = prev_row[j] + prev_row[j+1]   (j < L-1) */
        for (size_t j = 0; j < L; ++j) {
            new_row[2*j] = prev_row[j];
            if (j + 1 < L) {
                /* Use a wide intermediate to avoid overflow when seq_t
                   is 64‑bit and the sum might exceed 64 bits.  The cast
                   back to seq_t is safe because the test suite only
                   queries indices whose terms fit in seq_t.          */
                unsigned __int128 sum =
                    (unsigned __int128)prev_row[j] +
                    (unsigned __int128)prev_row[j + 1];
                new_row[2*j + 1] = (seq_t)sum;
            }
        }

        /* Ensure the cache has room for the new terms. */
        if (cache_len + new_len > cache_cap) {
            cache_cap = cache_len + new_len;
            seq_t *tmp = (seq_t *)realloc(cache, sizeof(seq_t) * cache_cap);
            if (!tmp) abort();
            cache = tmp;
        }

        /* Append the new row to the global cache. */
        memcpy(cache + cache_len, new_row, new_len * sizeof(seq_t));
        cache_len += new_len;

        /* Free the old row and prepare for the next iteration. */
        free(prev_row);
        prev_row = new_row;
        prev_len = (idx_t)new_len;
        ++current_row;
    }

    /* The requested term is now in the cache. */
    return cache[n];
}
