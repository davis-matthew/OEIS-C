/*
 * A080353  a(1)=5; for n>1, a(n)=a(n-1)+1 if n is already in the sequence,
 *          a(n)=a(n-1)+2 otherwise.
 * https://oeis.org/A080353
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, changed
 * Formula:   not stated
 * Range:     u8 n <= 217, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A080353
 * Recurrence:
 *   a(1) = 5
 *   for n > 1
 *     a(n) = a(n-1) + 1   if n is already a term in the sequence,
 *     a(n) = a(n-1) + 2   otherwise
 * The function keeps a cache of computed values and a boolean table
 * telling whether a number has appeared as a term.
 * Complexity: O(n) for extending the cache to a new maximum index;
 * previous entries remain cached for arbitrary orders of calls.
 */

#include <stdlib.h>
#include <string.h>

seq_t a(idx_t n)
{
    /* static cache of values and a table telling whether a number
       has already appeared as a term              */
    static seq_t *cache      = NULL;
    static idx_t cur_max_n   = 0;      /* largest n computed so far      */
    static uint8_t *seen     = NULL;   /* 1 if value has appeared before */
    static idx_t  seen_size  = 0;      /* current capacity of seen array */

    /* --- initialisation ------------------------------------------------- */
    if (cur_max_n == 0) {
        /* cache[0] unused, cache[1] = a(1) = 5 */
        cache = (seq_t*)malloc(2 * sizeof(seq_t));
        if (!cache) abort();
        cache[0] = (seq_t)0;      /* unused */
        cache[1] = (seq_t)5;      /* a(1) = 5 */
        cur_max_n = 1;

        /* seen array must contain index 5 (value 5 already present) */
        seen_size = 8;            /* at least 8 bits, > 5       */
        seen = (uint8_t*)calloc(seen_size, 1);
        if (!seen) abort();
        seen[5] = 1;              /* value 5 appears in the sequence */
    }

    /* --------------------------------------------------------------- */
    /* If the requested index is already known, just return it. */
    if (n <= cur_max_n)
        return cache[n];

    /* --------------------------------------------------------------- */
    /* Grow the cache array to accommodate the new index. */
    idx_t new_cache_size = n + 1;
    seq_t *new_cache = (seq_t*)realloc(cache, new_cache_size * sizeof(seq_t));
    if (!new_cache) abort();
    cache = new_cache;
    cache[0] = (seq_t)0;          /* keep unused element for 0 */

    /* --------------------------------------------------------------- */
    /* Ensure the seen array is large enough to index up to `n`
       (for the condition seen[n]). */
    if (seen_size <= n) {
        idx_t new_seen_size = seen_size ? seen_size << 1 : 8;
        while (new_seen_size <= n) new_seen_size <<= 1;
        uint8_t *tmp = (uint8_t*)realloc(seen, new_seen_size);
        if (!tmp) abort();
        memset(tmp + seen_size, 0, new_seen_size - seen_size);
        seen = tmp;
        seen_size = new_seen_size;
    }

    /* --------------------------------------------------------------- */
    /* Compute all terms up to the requested index. */
    for (idx_t i = cur_max_n + 1; i <= n; ++i) {
        seq_t prev = cache[i - 1];
        /* add 1 if index i is already present, else add 2   */
        seq_t step = seen[i] ? (seq_t)1 : (seq_t)2;
        seq_t val  = prev + step;
        cache[i] = val;

        /* mark this value as present for future indices */
        idx_t val_idx = (idx_t)val;
        if (val_idx >= seen_size) {
            idx_t new_seen_size = seen_size ? seen_size << 1 : 8;
            while (new_seen_size <= val_idx) new_seen_size <<= 1;
            uint8_t *tmp = (uint8_t*)realloc(seen, new_seen_size);
            if (!tmp) abort();
            memset(tmp + seen_size, 0, new_seen_size - seen_size);
            seen = tmp;
            seen_size = new_seen_size;
        }
        seen[val_idx] = 1;
    }

    cur_max_n = n;
    return cache[n];
}
