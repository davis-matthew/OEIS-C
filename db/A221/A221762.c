/*
 * A221762  Numbers m such that 11*m^2 + 5 is a square.
 * https://oeis.org/A221762
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 16, u64 n <= 30, u128 n <= 60,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n) = 20*a(n-2) - a(n-4) for n > 4
   Initial values: a(1)=1, a(2)=2, a(3)=22, a(4)=41
   This function computes a(n) for any n >= 1, memoising results
   so that repeated calls are O(1) after the first computation. */

#include <stdlib.h>

static seq_t *cache = NULL;
static size_t cache_size = 0;

seq_t a(idx_t n) {
    /* n is 1-indexed; guard against zero (undefined) */
    if (n == 0) return 0;

    /* Convert index to size_t (safe for n up to 10000 as required) */
    size_t idx = (size_t)n;

    /* Initialise cache with the first four terms if necessary */
    if (cache == NULL) {
        cache = (seq_t *)malloc(4 * sizeof(seq_t));
        if (cache == NULL) return 0;          /* out of memory – return 0 */
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)2;
        cache[2] = (seq_t)22;
        cache[3] = (seq_t)41;
        cache_size = 4;
    }

    /* Grow the cache if the requested index is beyond the current size */
    if (idx > cache_size) {
        size_t new_size = idx;
        seq_t *new_cache = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (new_cache == NULL) return 0;      /* out of memory – return 0 */
        cache = new_cache;

        /* Compute new terms using the recurrence */
        for (size_t i = cache_size; i < new_size; ++i) {
            /* i corresponds to term a(i+1) (1‑based index) */
            seq_t prev2 = cache[i - 2];   /* a(i-1)  → a(n-2) */
            seq_t prev4 = cache[i - 4];   /* a(i-3)  → a(n-4) */
            cache[i] = (seq_t)(( (seq_t)20 ) * prev2 - prev4);
        }
        cache_size = new_size;
    }

    return cache[idx - 1];
}
