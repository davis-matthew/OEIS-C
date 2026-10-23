/*
 * A016215  Expansion of 1 / ((1-x) * (1-3*x) * (1-10*x)).
 * https://oeis.org/A016215
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0) = 1, a(1) = 14, a(n) = 13*a(n-1) - 30*a(n-2) + 1 (n >= 2)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 19, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Cached values of the sequence */
static seq_t *cache = NULL;
static size_t cap   = 0;   /* current allocated capacity */
static idx_t last_idx = 0; /* last computed index */

/* Ensure the internal cache can hold values up to index n */
static void grow_cache(idx_t n) {
    if ((size_t)n + 1 <= cap)
        return;

    size_t new_cap = cap ? cap * 2 : 2;          /* start with at least 2 */
    while ((size_t)n + 1 > new_cap)
        new_cap *= 2;

    seq_t *new_cache = (seq_t *)malloc(new_cap * sizeof(seq_t));
    if (!new_cache)
        return; /* abort on allocation failure – the caller will crash later */

    if (cache) {
        for (size_t i = 0; i < cap; ++i)
            new_cache[i] = cache[i];
        free(cache);
    }
    cache = new_cache;
    cap   = new_cap;
}

seq_t a(idx_t n) {
    /* Initialise cache with the first two terms if empty */
    if (cap == 0) {
        cache     = (seq_t *)malloc(2 * sizeof(seq_t));
        cache[0]  = (seq_t)1;
        cache[1]  = (seq_t)14;
        last_idx  = 1;
        cap       = 2;
    }

    /* If the requested term is already computed, return it */
    if ((size_t)n <= last_idx)
        return cache[n];

    /* Grow cache to accommodate up to n */
    grow_cache(n);

    /* Compute missing values from last_idx+1 up to n */
    for (idx_t i = last_idx + 1; i <= n; ++i) {
        cache[i] = 13 * cache[i - 1] - 30 * cache[i - 2] + (seq_t)1;
    }
    last_idx = n;

    return cache[n];
}
