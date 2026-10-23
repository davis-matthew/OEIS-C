/*
 * A009928  Coordination sequence for Cr3Si, Cr position.
 * https://oeis.org/A009928
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 65, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence for a(n) of A009928:
   a(n) = a(n-1) - a(n-2) + 2*a(n-3) - 2*a(n-7) + a(n-8) - a(n-9) + a(n-10)
   for n >= 14.
   Base terms (0 ≤ n < 14) are provided directly. */

#include <stdlib.h>

static const seq_t init[14] = {
    /* a(0) … a(13) */
    1, 14, 50, 114, 214, 350, 510, 710, 930, 1186,
    1474, 1798, 2142, 2526
};

seq_t a(idx_t n) {
    /* Static cache for memoisation */
    static seq_t *cache = NULL;      /* array of previously computed values   */
    static idx_t  cache_cap = 0;      /* capacity of the cache array           */
    static idx_t  last_computed = 13; /* highest index that has been computed  */

    /* Return the base value for small indices */
    if (n < 14)
        return init[n];

    /* Initialise the cache on the first call */
    if (cache == NULL) {
        cache_cap = 20; /* initial capacity (≥14) */
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache) return 0; /* allocation failure – unlikely for the test harness */
        for (idx_t i = 0; i < 14; ++i)
            cache[i] = init[i];
    }

    /* Extend the cache if necessary */
    if (n >= cache_cap) {
        size_t new_cap = cache_cap * 2;
        if (new_cap < (size_t)(n + 1))
            new_cap = n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return 0; /* allocation failure */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Compute the missing values up to index n */
    for (idx_t i = last_computed + 1; i <= n; ++i) {
        /* recurrence: a(i) = a(i-1) - a(i-2) + 2*a(i-3) - 2*a(i-7)
                         + a(i-8) - a(i-9) + a(i-10) */
        seq_t val = cache[i-1] - cache[i-2] + (seq_t)2 * cache[i-3]
                    - (seq_t)2 * cache[i-7] + cache[i-8]
                    - cache[i-9] + cache[i-10];
        cache[i] = val;
    }
    last_computed = n;
    return cache[n];
}
