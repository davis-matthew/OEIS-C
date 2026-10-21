/*
 * A059294  2-boustrophedon transform applied to 1, 0, 0, 0, ...
 * https://oeis.org/A059294
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 12, u64 n <= 19, u128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   • Recurrence: a(0) = 1. 
   • For n > 0, a(n) equals the bottom‑right entry of the (n‑th) row of a
     lower‑triangular array T defined by 
        T(r,0) = (r==0 ? 1 : 0),                     // s(r)
        T(r,c) = T(r,c-1) + T(r-1,r-c) + T(r-1,r-c-1)   // for 1 ≤ c ≤ r.
   • The 2‑boustrophedon transform of the sequence 1,0,0,0,… is exactly the
     OEIS sequence A059294.
*/

#include <stdlib.h>

/* Computes the n-th term of OEIS sequence A059294. */
seq_t a(idx_t n) {
    /* --- Static state shared across calls -------------------------------- */
    static seq_t *cache   = NULL;    /* a(0)…a(max_computed-1) */
    static size_t cache_len = 0;     /* Number of cached terms */
    static seq_t *row_prev = NULL;   /* Previously computed row of T */
    static seq_t *row_curr = NULL;   /* Current row being built */
    static size_t row_cap = 0;       /* Capacity of the two row buffers */

    /* --- Initialisation on first use ------------------------------------- */
    if (cache == NULL) {
        /* Allocate cache for a(0) */
        cache = (seq_t *)calloc(1, sizeof(seq_t));
        if (!cache) return (seq_t)0;                     /* allocation fail */
        cache[0] = (seq_t)1;
        cache_len = 1;

        /* Allocate two row buffers for row 0 */
        row_cap = 1;
        row_prev = (seq_t *)calloc(row_cap, sizeof(seq_t));
        row_curr = (seq_t *)calloc(row_cap, sizeof(seq_t));
        if (!row_prev || !row_curr) return (seq_t)0;     /* allocation fail */
        row_prev[0] = (seq_t)1;    /* T(0,0)=1 */
    }

    /* --- Return if already cached ---------------------------------------- */
    if (n < cache_len)
        return cache[n];

    /* Preserve number of cached terms before growing the cache. */
    size_t old_cache_len = cache_len;

    /* --- Grow the cache to hold indices up to n -------------------------- */
    if ((size_t)n + 1 > cache_len) {
        seq_t *tmp = (seq_t *)realloc(cache, ((size_t)n + 1) * sizeof(seq_t));
        if (!tmp) return (seq_t)0;                     /* allocation fail */
        cache = tmp;
        /* cache_len remains old_cache_len; newly allocated slots are
           uninitialized and will be filled in the loop below. */
    }

    /* --- Ensure the row buffers have sufficient capacity ---------------- */
    if ((size_t)n + 1 > row_cap) {
        size_t new_cap = (size_t)n + 1;
        seq_t *tmp_prev = (seq_t *)realloc(row_prev, new_cap * sizeof(seq_t));
        seq_t *tmp_curr = (seq_t *)realloc(row_curr, new_cap * sizeof(seq_t));
        if (!tmp_prev || !tmp_curr) return (seq_t)0;    /* allocation fail */
        row_prev = tmp_prev;
        row_curr = tmp_curr;
        row_cap = new_cap;
    }

    /* --- Build rows incrementally from old_cache_len up to n -------------- */
    for (idx_t r = old_cache_len; r <= n; ++r) {
        /* first element of row r */
        row_curr[0] = (r == 0) ? (seq_t)1 : (seq_t)0;   /* s(r) */

        for (idx_t c = 1; c <= r; ++c) {
            seq_t left  = row_curr[c - 1];
            seq_t up1   = row_prev[r - c];      /* r-c ≤ r-1, valid index */
            seq_t up2   = 0;
            if (c < r)                          /* r-c-1 ≥ 0  */
                up2 = row_prev[r - c - 1];

            row_curr[c] = left + up1 + up2;
        }

        /* Store the last element of this row: a(r) = T(r,r) */
        cache[r] = row_curr[r];

        /* Prepare for next iteration: swap row_prev and row_curr */
        seq_t *tmp = row_prev;
        row_prev = row_curr;
        row_curr = tmp;
    }

    /* Update the cache length now that the new terms have been computed. */
    cache_len = (size_t)n + 1;

    /* Return the requested term. */
    return cache[n];
}
