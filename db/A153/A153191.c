/*
 * A153191  a(n) = 9*a(n-1) + 6*a(n-2); a(0)=0, a(1)=1.
 * https://oeis.org/A153191
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 9*a(n-1) + 6*a(n-2), a(0)=0, a(1)=1
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 10, u64 n <= 20, u128 n <= 40,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    /* static cache to memoize computed terms across calls */
    static seq_t *cache = NULL;
    static idx_t cache_cap = 0;   /* allocated size of cache */
    static idx_t last_computed = 1; /* highest index already computed */

    /* base cases */
    if (n == 0) return (seq_t)0;
    if (n == 1) return (seq_t)1;

    /* initialise cache on first call */
    if (!cache) {
        cache_cap = 4;                      /* start with a few slots */
        cache = (seq_t*)malloc(cache_cap * sizeof(seq_t));
        if (!cache) exit(EXIT_FAILURE);
        cache[0] = (seq_t)0;
        cache[1] = (seq_t)1;
        /* leave remaining slots uninitialised until needed */
        last_computed = 1;
    }

    /* extend cache size if necessary */
    if (n >= cache_cap) {
        idx_t new_cap = cache_cap;
        while (new_cap <= n) new_cap <<= 1;   /* double until enough */
        seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) exit(EXIT_FAILURE);
        cache = tmp;
        cache_cap = new_cap;
    }

    /* compute missing terms using the recurrence */
    for (idx_t i = last_computed + 1; i <= n; ++i) {
        seq_t prev1 = cache[i - 1];
        seq_t prev2 = cache[i - 2];
        cache[i] = (seq_t)9 * prev1 + (seq_t)6 * prev2;
    }
    last_computed = n;

    return cache[n];
}
