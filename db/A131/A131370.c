/*
 * A131370  a(n) = 3a(n-1) - 3a(n-2) + 2a(n-3), a(0) = 3, a(1) = 2, a(2) = 0.
 * https://oeis.org/A131370
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3a(n-1) - 3a(n-2) + 2a(n-3), a(0)=3, a(1)=2, a(2)=0
 * Range:     u8 n <= 9, u16 n <= 17, u32 n <= 33, u64 n <= 65, u128 n <= 129,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* External linkage required by the test harness */
seq_t a(idx_t n)
{
    /* static cache for previously computed values */
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;   /* number of elements currently stored */

    /* Helper to ensure enough space for an array of size n+1 */
    if (cache == NULL) {
        /* allocate space for the initial 3 values */
        size_t init_cap = (n >= 2) ? (size_t)(n + 1) : 3;
        cache = (seq_t *)malloc(init_cap * sizeof(seq_t));
        if (cache == NULL) exit(EXIT_FAILURE); /* abort on allocation failure */
        /* initialise the first three terms */
        cache[0] = (seq_t)3;
        cache[1] = (seq_t)2;
        cache[2] = (seq_t)0;
        cache_size = 3;
    }

    /* grow the cache if needed */
    if ((idx_t)cache_size <= n) {
        /* Determine new capacity (enough for indices 0..n) */
        size_t new_cap = (size_t)(n + 1);
        if (new_cap > (size_t)cache_size) {
            /* Reallocate the cache to hold the new capacity */
            seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            if (tmp == NULL) exit(EXIT_FAILURE);
            cache = tmp;
        }

        /* Compute missing terms using the recurrence:
           a(i) = 3*(a(i-1)-a(i-2)) + 2*a(i-3)  for i >= 3
           The expression a(i-1)-a(i-2) is guaranteed non‑negative,
           so unsigned underflow cannot occur. */
        for (idx_t i = cache_size; i <= n; ++i) {
            seq_t diff = cache[i - 1] - cache[i - 2];
            cache[i] = 3 * diff + 2 * cache[i - 3];
        }
        cache_size = n + 1;
    }

    return cache[n];
}
