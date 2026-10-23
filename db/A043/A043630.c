/*
 * A043630  Numbers whose base-8 representation has exactly 8 runs.
 * https://oeis.org/A043630
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = the n-th positive integer whose base-8 representation has
 *            exactly 8 runs (a run is a maximal block of equal digits)
 * Range:     u8 none, u16 none, u32 n <= 24, u64 n <= 24, u128 n <= 24,
 *            big any n
 * Helpers:   run_count_oct
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>
#include "../utils/run_count_oct.h"

/* Dynamic cache of already found terms. */
static seq_t *cache = NULL;      /* pointer to cache array */
static size_t cache_cap = 0;      /* allocated capacity */
static size_t cache_len = 0;      /* number of terms stored */
static seq_t last_checked = (seq_t)0; /* largest integer examined so far */

/* Append a new term to the cache, resizing if necessary. */
static void cache_append(seq_t val) {
    if (cache_len == cache_cap) {
        size_t new_cap = (cache_cap == 0) ? 64 : (cache_cap << 1);
        seq_t *new_ptr = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!new_ptr) {
            /* In case of allocation failure, abort. */
            abort();
        }
        cache = new_ptr;
        cache_cap = new_cap;
    }
    cache[cache_len++] = val;
}

/* Main sequence function. */
seq_t a(idx_t n) {
    if (n == 0)  /* offset is 1, but ignore n==0 */
        return (seq_t)0;

    /* Grow the cache until we have at least n terms. */
    while (cache_len < (size_t)n) {
        seq_t cur = last_checked + (seq_t)1;
        /* Scan until we find the next term. */
        if (cur == (seq_t)0)  /* overflow protection (unlikely) */
            abort();
        if (run_count_oct(cur) == 8) {
            cache_append(cur);
        }
        last_checked = cur;
    }
    return cache[(size_t)(n - 1)];
}
