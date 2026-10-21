/*
 * A073591  a(n) = A000522(n) + 1.
 * https://oeis.org/A073591
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n*(a(n-1)-1)+2, a(0)=2 (OEIS A073591, where
 *            a(n)=A000522(n)+1)
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 12, u64 n <= 20, u128 n <= 33,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* External types defined by the build */
extern seq_t a(idx_t n);

seq_t a(idx_t n) {
    /* Static cache for previously computed values */
    static seq_t *cache = NULL;  /* Array of computed a(i) up to cache_size-1 */
    static idx_t cache_size = 0; /* Number of values stored in cache (cache_size = last_index+1) */

    /* Initialize cache with a(0) if empty */
    if (cache_size == 0) {
        cache = (seq_t*)malloc(sizeof(seq_t));
        if (!cache) return (seq_t)0;                /* Allocation failure, return 0 as safety */
        cache[0] = (seq_t)2;                       /* a(0) = 2 */
        cache_size = 1;
    }

    /* Extend cache up to requested n if necessary */
    if (n >= cache_size) {
        /* Reallocate cache to hold values up to n */
        seq_t *tmp = (seq_t*)realloc(cache, sizeof(seq_t) * (n + 1));
        if (!tmp) return (seq_t)0;                 /* Allocation failure, return 0 as safety */
        cache = tmp;
        /* Compute missing terms using recurrence */
        for (idx_t i = cache_size; i <= n; ++i) {
            seq_t prev = cache[i - 1];
            cache[i] = ((seq_t)i) * (prev - 1) + 2;
        }
        cache_size = n + 1;
    }

    return cache[n];
}
