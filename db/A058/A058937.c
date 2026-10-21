/*
 * A058937  Maximal exponent of x in all terms of Somos polynomial of order n.
 * https://oeis.org/A058937
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 + a(n-1) + a(n-3) – a(n-4) for n ≥ 6 Initial values
 *            (offset 1): a(1)=1, a(2)=a(3)=a(4)=a(5)=0 (The recurrence is
 *            valid for all n ≥ 6. Calls with smaller n return the listed base
 *            values.) The function caches computed values so that calls in
 *            arbitrary order are fast
 * Range:     u8 n <= 42, u16 n <= 61, u32 n <= 61, u64 n <= 61, u128 n <= 61,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>   /* for malloc, realloc, free */

seq_t a(idx_t n)
{
    /* static cache of previously computed values */
    static seq_t *cache = NULL;      /* 1‑based indexing: cache[1] = a(1) */
    static idx_t cache_capacity = 0;
    static idx_t cache_size     = 0;  /* highest index stored */

    /* ensure cache has room for indices up to n */
    if (n > cache_size) {
        idx_t needed = n + 1; /* +1 because we ignore index 0 */
        if (needed > cache_capacity) {
            /* grow the cache: double until it fits, or to needed */
            idx_t new_cap = cache_capacity == 0 ? 16 : cache_capacity;
            while (new_cap < needed) new_cap <<= 1U;
            seq_t *new_cache = (seq_t *)realloc(cache, (size_t)new_cap * sizeof(seq_t));
            if (new_cache == NULL) exit(1); /* fatal if out of memory */
            cache = new_cache;
            cache_capacity = new_cap;
        }
        /* compute new entries from cache_size+1 up to n */
        for (idx_t i = cache_size + 1; i <= n; ++i) {
            if (i == 1) {
                cache[i] = (seq_t)1;
            } else if (i <= 5) {
                cache[i] = (seq_t)0;
            } else {
                /* recurrence: a(i) = 1 + a(i-1) + a(i-3) - a(i-4) */
                cache[i] = (seq_t)( (seq_t)1 + cache[i-1] + cache[i-3] - cache[i-4] );
            }
        }
        cache_size = n;
    }
    return cache[n];
}
