/*
 * A207436  Number of n X 2 0..1 arrays avoiding 0 0 1 and 0 1 0 horizontally
 *          and 0 0 1 and 1 0 0 vertically.
 * https://oeis.org/A207436
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 4*a(n-1) - 2*a(n-2) - 6*a(n-3) + 4*a(n-4) + 2*a(n-5) -
 *            a(n-6) with initial values a(1)=4, a(2)=16, a(3)=36, a(4)=81,
 *            a(5)=196, a(6)=484, a(7)=1225 (n ≥ 1) This implementation
 *            memoises previously computed terms and, if needed, grows the
 *            cache using malloc/realloc (C) or new/delete (C++)
 * Range:     u8 n <= 5, u16 n <= 11, u32 n <= 22, u64 n <= 45, u128 n <= 91,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#ifndef __cplusplus
#include <stdlib.h>
#include <string.h>
#endif

seq_t a(idx_t n)
{
    /* Cached values, initialised on first call */
    static idx_t cache_len = 0;   /* number of elements stored (>= 7 after init) */
    static seq_t *cache = NULL;   /* 0‑based array, cache[i] = a(i+1) */

    if (cache_len == 0) {    /* first initialisation */
        cache_len = 7;
#if defined(__cplusplus)
        cache = new seq_t[cache_len];
#else
        cache = (seq_t*)malloc(cache_len * sizeof(seq_t));
#endif
        cache[0] = (seq_t)4;
        cache[1] = (seq_t)16;
        cache[2] = (seq_t)36;
        cache[3] = (seq_t)81;
        cache[4] = (seq_t)196;
        cache[5] = (seq_t)484;
        cache[6] = (seq_t)1225;
    }

    if (n <= cache_len)          /* already computed */
        return cache[n - 1];

    /* Figure out how many we need to compute */
    idx_t old_len = cache_len;

    /* Grow the cache if necessary */
    if (cache_len < n) {
        idx_t new_len = n;
#if defined(__cplusplus)
        seq_t *new_cache = new seq_t[new_len];
        for (idx_t i = 0; i < old_len; ++i)
            new_cache[i] = cache[i];
        delete[] cache;
#else
        seq_t *new_cache = (seq_t*)malloc(new_len * sizeof(seq_t));
        memcpy(new_cache, cache, old_len * sizeof(seq_t));
        free(cache);
#endif
        cache = new_cache;
        cache_len = new_len;
    }

    /* Compute missing terms */
    for (idx_t i = old_len; i < n; ++i) {
        seq_t val = (seq_t)4 * cache[i - 1]
                 - (seq_t)2 * cache[i - 2]
                 - (seq_t)6 * cache[i - 3]
                 + (seq_t)4 * cache[i - 4]
                 + (seq_t)2 * cache[i - 5]
                 -          cache[i - 6];
        cache[i] = val;
    }

    return cache[n - 1];
}
