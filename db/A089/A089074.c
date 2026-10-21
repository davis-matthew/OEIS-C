/*
 * A089074  Expansion of x*(1 + x + x^2)/(1 - 2*x + x^5).
 * https://oeis.org/A089074
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*a(n-1) - a(n-5) for n >=5; a(0)=0, a(1)=1, a(2)=3,
 *            a(3)=7, a(4)=14, a(5)=28
 * Range:     u8 n <= 8, u16 n <= 16, u32 n <= 33, u64 n <= 67, u128 n <= 135,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    /* Static cache to allow out‑of‑order calls. */
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;    /* number of elements stored (indexes 0..cache_size-1) */
    static idx_t cache_capacity = 0;

    /* Allocate or expand the cache as needed. */
    if (cache == NULL) {
        /* Ensure at least 6 elements for the first terms. */
        cache_capacity = (n < 5) ? 6 : (n + 1);
        cache = (seq_t *)malloc(cache_capacity * sizeof(seq_t));
        if (!cache) return (seq_t)0;  /* fallback in unlikely out‑of‑memory case */

        /* Initialize the first six terms. */
        cache[0] = 0;
        cache[1] = 1;
        cache[2] = 3;
        cache[3] = 7;
        cache[4] = 14;
        cache[5] = 28;
        cache_size = 6;
    }

    /* Grow the cached array if the requested index exceeds the current capacity. */
    if (n >= cache_capacity) {
        idx_t new_capacity = cache_capacity * 2;
        if (new_capacity <= n) new_capacity = n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_capacity * sizeof(seq_t));
        if (!tmp) return (seq_t)0;  /* fallback in unlikely out‑of‑memory case */
        cache = tmp;
        cache_capacity = new_capacity;
    }

    /* Compute missing terms up to index n. */
    for (idx_t i = cache_size; i <= n; ++i) {
        /* For n < 5 the recurrence is not used; the array already holds the values. */
        if (i >= 6) {
            seq_t two = cache[i - 1] << 1;      /* 2 * cache[i-1] */
            cache[i] = two - cache[i - 5];      /* a(i) = 2*a(i-1) - a(i-5) */
        } else {
            /* For i == 5 we already have the value. */
        }
    }
    if (n >= cache_size) cache_size = n + 1;

    return cache[n];
}
