/*
 * A221175  a(0)=-5, a(1)=6; thereafter a(n) = 2*a(n-1) + a(n-2).
 * https://oeis.org/A221175
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   not stated
 * Range:     i8 n <= 5, i16 n <= 11, i32 n <= 24, i64 n <= 49, i128 n <= 99,
 *            big any n
 * Validated: standard, pass, 46 terms through n = 953 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0) = -5, a(1) = 6; a(n) = 2*a(n-1) + a(n-2) for n>=2 */
#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include "../utils/mem.h"

static seq_t *alloc_seq_array(size_t n)
{
    return mem_new(seq_t, n);
}

static void free_cache(seq_t *p)
{
    mem_free(p);
}

seq_t a(idx_t n) {
    static seq_t *cache = NULL;   /* array of computed values           */
    static size_t  cache_cap = 0; /* capacity of the array              */
    static idx_t   cached_len = 0;/* number of values stored in cache   */

    /* Return already computed value if it exists. */
    if (n < cached_len)
        return cache[n];

    /* Grow the array if needed. */
    if ((size_t)n + 1 > cache_cap) {
        size_t new_cap = (cache_cap == 0) ? 2 : cache_cap;
        while (new_cap <= (size_t)n + 1)
            new_cap *= 2;

        seq_t *new_cache = alloc_seq_array(new_cap);
        if (!new_cache)
            return (seq_t)0; /* unlikely, but prevents crash */

        /* Copy existing data. */
        if (cache) {
            for (size_t i = 0; i < cached_len; ++i)
                new_cache[i] = cache[i];
            free_cache(cache);
        }
        cache      = new_cache;
        cache_cap  = new_cap;
    }

    /* Initialise first two values if the cache is empty. */
    if (cached_len == 0) {
        cache[0] = (seq_t)-5;
        cached_len = 1;
    }
    if (cached_len == 1) {
        cache[1] = (seq_t)6;
        cached_len = 2;
    }

    /* Compute values up to n. */
    for (idx_t i = cached_len; i <= n; ++i) {
        cache[i] = (seq_t)2 * cache[i - 1] + cache[i - 2];
    }
    cached_len = n + 1;
    return cache[n];
}
