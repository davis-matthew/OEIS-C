/*
 * A052536  Number of compositions of n when parts 1 and 2 are of two kinds.
 * https://oeis.org/A052536
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 21, u64 n <= 42, u128 n <= 84,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Formula: a(n) = 3*a(n-1) - a(n-3)  (n >= 3)
 * Initial values: a(0) = 1, a(1) = 2, a(2) = 6
 *
 * This implementation caches previously computed terms to allow
 * arbitrary order of calls.  It grows the cache dynamically
 * and computes only the missing entries using the linear recurrence.
 */

#include <stdlib.h>
#include <stddef.h>

seq_t a(idx_t n) {
    /* Static cache for sequence values */
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;       /* number of allocated slots */
    static idx_t computed_upto = 2;    /* highest index that has been computed */

    /* Helper to ensure the cache has space for up to required index */
    if (cache == NULL) {
        /* Initial allocation – more than base cases to reduce reallocs */
        cache_size = 8;
        cache = (seq_t *)malloc(cache_size * sizeof(seq_t));
        if (!cache) abort();

        /* Initialize base cases */
        cache[0] = 1;
        cache[1] = 2;
        cache[2] = 6;
        for (idx_t i = 3; i < cache_size; ++i)
            cache[i] = 0;
    }

    /* Grow cache if necessary */
    if (n + 1 > cache_size) {
        idx_t new_size = cache_size * 2;
        if (new_size <= n + 1) new_size = n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (!tmp) abort();
        cache = tmp;
        for (idx_t i = cache_size; i < new_size; ++i)
            cache[i] = 0;
        cache_size = new_size;
    }

    /* Compute any missing values up to n */
    if (n > computed_upto) {
        for (idx_t i = computed_upto + 1; i <= n; ++i) {
            cache[i] = 3 * cache[i - 1] - cache[i - 3];
        }
        computed_upto = n;
    }

    return cache[n];
}
