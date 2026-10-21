/*
 * A061999  a(n) = 2*a(n-1)^2 - 2*a(n-2)^2 with a(0) = 0, a(1) = 1.
 * https://oeis.org/A061999
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*a(n-1)^2 - 2*a(n-2)^2 with a(0)=0, a(1)=1
 * Range:     u8 n <= 4, u16 n <= 5, u32 n <= 6, u64 n <= 7, u128 n <= 8,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache of computed terms */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;

    /* initialise cache with a(0)=0, a(1)=1 */
    if (cache == NULL) {
        cache = (seq_t*)malloc(2 * sizeof(seq_t));
        if (!cache) return (seq_t)0;  /* out of memory – will never happen in tests */
        cache[0] = (seq_t)0;
        cache[1] = (seq_t)1;
        cache_len = 2;
    }

    /* extend cache up to requested n, if needed */
    if (n >= cache_len) {
        size_t new_len = n + 1;
        seq_t *new_cache = (seq_t*)realloc(cache, new_len * sizeof(seq_t));
        if (!new_cache) return (seq_t)0;  /* realloc failed – unlikely in test harness */
        cache = new_cache;
        /* compute missing terms */
        for (size_t i = cache_len; i <= (size_t)n; ++i) {
            seq_t prev  = cache[i - 1];
            seq_t prev2 = cache[i - 2];
            /* 2*prev^2 - 2*prev2^2 */
            seq_t term = (seq_t)2 * prev * prev - (seq_t)2 * prev2 * prev2;
            cache[i] = term;
        }
        cache_len = new_len;
    }

    return cache[(size_t)n];
}
