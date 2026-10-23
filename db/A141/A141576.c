/*
 * A141576  a(0)=-1, a(1)=0, a(2)=1, a(n) = a(n-1) - 2*a(n-2) + a(n-3).
 * https://oeis.org/A141576
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = a(n-1) - 2*a(n-2) + a(n-3) with initial values a(0) = -1,
 *            a(1) = 0, a(2) = 1
 * Range:     i8 n <= 19, i16 n <= 39, i32 n <= 79, i64 n <= 157,
 *            i128 n <= 315, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    /* Static cache for memoisation */
    static seq_t *cache = NULL;
    static idx_t cached_up_to = 0;
    static int initialized = 0;

    /* Initialise base values on first call */
    if (!initialized) {
        cache = (seq_t*)malloc(3 * sizeof(seq_t));
        if (!cache) return 0; /* memory failure – unlikely on small n */
        cache[0] = (seq_t)-1;
        cache[1] = (seq_t)0;
        cache[2] = (seq_t)1;
        cached_up_to = 2;
        initialized = 1;
    }

    if (n <= cached_up_to) {
        return cache[(size_t)n];
    }

    /* Expand the cache to accommodate index n */
    idx_t new_cap = n + 1;
    seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp) return 0; /* memory failure – unlikely on moderate n */
    cache = tmp;

    /* Compute missing terms using the recurrence */
    for (idx_t i = cached_up_to + 1; i <= n; ++i) {
        cache[(size_t)i] = cache[(size_t)(i - 1)]
                          - (seq_t)(2) * cache[(size_t)(i - 2)]
                          + cache[(size_t)(i - 3)];
    }
    cached_up_to = n;
    return cache[(size_t)n];
}
