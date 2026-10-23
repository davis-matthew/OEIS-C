/*
 * A143391  A binomial recursion sequence: a(n+1) = binomial(a(n),n), with
 *          a(1) = 4.
 * https://oeis.org/A143391
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 5, u32 n <= 5, u64 n <= 6, u128 n <= 6,
 *            big any n
 * Helpers:   binomial_large_k
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence:  a(1) = 4,  a(n+1) = binomial(a(n), n)  for n ≥ 1. */
#include <stdlib.h>
#include "../utils/binomial_large_k.h"

/* Ensure the cache can hold indices 0 … n. */
static void ensure_cache(idx_t n, seq_t **cache_ptr, idx_t *cache_size) {
    if ((size_t)(n + 1) <= (size_t)*cache_size)
        return;
    size_t new_cap = (size_t)(n + 1);
    seq_t *tmp = (seq_t*)realloc(*cache_ptr, new_cap * sizeof(seq_t));
    if (tmp == NULL) {
        /* Allocation failure – caller cannot compute further values. */
        abort();
    }
    *cache_ptr = tmp;
    *cache_size = (idx_t)new_cap;
}

/* External linkage as required. */
seq_t a(idx_t n) {
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;      /* allocated size (max index + 1) */
    static idx_t computed_up_to = 0;  /* last index computed */

    if (n < (idx_t)1)
        return (seq_t)0;  /* undefined for n <= 0 */

    /* Allocate or grow the cache for indices up to n. */
    ensure_cache(n, &cache, &cache_size);

    /* Initialise base value(s). */
    if (computed_up_to < (idx_t)1) {
        cache[(idx_t)1] = (seq_t)4;
        computed_up_to = (idx_t)1;
    }
    if (n == (idx_t)1)
        return cache[(idx_t)1];

    /* Ensure a(2) is available. */
    if (computed_up_to < (idx_t)2) {
        cache[(idx_t)2] = binomial_large_k(cache[(idx_t)1], 1); /* binomial(4,1)=4 */
        computed_up_to = (idx_t)2;
    }
    if (n == (idx_t)2)
        return cache[(idx_t)2];

    /* Compute any missing terms up to n. */
    for (idx_t i = computed_up_to + 1; i <= n; ++i) {
        /* a(i) = binomial(a(i-1), i-1) */
        cache[i] = binomial_large_k(cache[i - 1], i - 1);
    }
    computed_up_to = n;
    return cache[n];
}
