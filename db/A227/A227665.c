/*
 * A227665  Number of lattice paths from {n}^3 to {0}^3 using steps that
 *          decrement one component by 1 such that for each point
 *          (p_1,p_2,p_3) we have abs(p_{i}-p_{i+1}) <= 1.
 * https://oeis.org/A227665
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 7*a(n-1) + 2*a(n-2) with a(0) = 1, a(1) = 6
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 11, u64 n <= 22, u128 n <= 44,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence derived from the generating function (x-1)/(2*x^2+7*x-1). */

#include <stdlib.h>

/* Forward declaration of the function with external linkage. */
seq_t a(idx_t n) {
    /* Static cache to store previously computed values. */
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;
    static idx_t cache_cap = 0;

    /* Ensure the cache is at least of size 2 to hold a(0) and a(1). */
    if (cache == NULL) {
        cache_cap = 2;
        cache = (seq_t *)calloc(cache_cap, sizeof(seq_t));
        if (!cache) return 0;          /* memory allocation failed */
        cache[0] = 1;                  /* a(0) */
        cache[1] = 6;                  /* a(1) */
        cache_size = 2;
    }

    /* If the requested index is already cached, return it. */
    if (n < cache_size)
        return cache[n];

    /* Extend the cache to accommodate index n. */
    for (idx_t i = cache_size; i <= n; ++i) {
        /* Grow the allocated storage if necessary. */
        if (i >= cache_cap) {
            size_t new_cap = cache_cap * 2;
            seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            if (!new_cache) return 0;  /* memory allocation failed */
            cache = new_cache;
            cache_cap = new_cap;
        }
        /* Compute the next value using the recurrence. */
        /* The recurrence applies for i >= 2, but the loop
           starts at cache_size which will always be >= 2 after
           initialization (a(0) and a(1) already stored). */
        cache[i] = (seq_t)7 * cache[i - 1] + (seq_t)2 * cache[i - 2];
    }

    /* Update the cached size. */
    cache_size = n + 1;

    return cache[n];
}
