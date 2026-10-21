/*
 * A209239  Number of length n words on {0,1,2} with no four consecutive 0's.
 * https://oeis.org/A209239
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 20, u64 n <= 25, u128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A209239
 * Recurrence: a(n) = 2*(a(n-1)+a(n-2)+a(n-3)+a(n-4))   for n >= 4
 * Initial values: a(0)=1, a(1)=3, a(2)=9, a(3)=27
 * Offset: 0
 */

#include <stddef.h>  /* for size_t */
#include <stdlib.h>  /* for malloc, realloc, free */

/* Static cache for memoisation */
static seq_t *cache = NULL;
static idx_t cache_len = 0;

/* Return the n-th term of OEIS A209239. */
seq_t a(idx_t n)
{
    /* First call or n outside current cache: create/resize */
    if (cache == NULL) {
        /* Determine cache size: need at least 4 elements for the base values */
        idx_t init_len = (n + 1 > 4) ? n + 1 : 4;
        cache = (seq_t*)calloc(init_len, sizeof(seq_t));
        cache_len = init_len;

        /* Base values */
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)3;
        cache[2] = (seq_t)9;
        cache[3] = (seq_t)27;
    } else if (n >= cache_len) {
        /* Grow the cache to accommodate n */
        cache = (seq_t*)realloc(cache, (n + 1) * sizeof(seq_t));
        for (idx_t i = cache_len; i <= n; ++i) {
            cache[i] = (seq_t)2 * (cache[i-1] + cache[i-2] +
                                   cache[i-3] + cache[i-4]);
        }
        cache_len = n + 1;
    }

    /* For n < cache_len but cache already initialised, values are ready */
    return cache[n];
}
