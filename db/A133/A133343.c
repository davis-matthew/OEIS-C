/*
 * A133343  a(n) = 2*a(n-1) + 13*a(n-2), for n>1, a(0)=1, a(1)=1.
 * https://oeis.org/A133343
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, changed
 * Formula:   a(n) = 2*a(n-1) + 13*a(n-2), a(0) = 1, a(1) = 1
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 14, u64 n <= 28, u128 n <= 57,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Static cache of computed values, grows as needed */
static seq_t *cache = NULL;
static idx_t cache_size = 0;

/* Computes a(n) for sequence A133343 */
seq_t a(idx_t n) {
    /* Initialize cache with base cases */
    if (cache == NULL) {
        /* Allocate space for at least two elements */
        cache_size = 2;
        cache = (seq_t *)malloc(sizeof(seq_t) * cache_size);
        cache[0] = 1;
        cache[1] = 1;
    }

    /* Grow cache if necessary and compute missing terms */
    if (n >= cache_size) {
        size_t new_size = (size_t)(n + 1);
        seq_t *new_cache = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (new_cache == NULL) {
            /* Handle allocation failure: abort program (or set to 0) */
            abort();
        }
        cache = new_cache;
        /* Compute terms from the first missing index up to n */
        for (idx_t i = cache_size; i <= n; ++i) {
            cache[i] = 2 * cache[i - 1] + 13 * cache[i - 2];
        }
        cache_size = new_size;
    }

    return cache[n];
}
