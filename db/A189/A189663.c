/*
 * A189663  Partial sums of A189661.
 * https://oeis.org/A189663
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(1+a(n-2)) + a(n-1-a(n-2)), a(1)=0, a(2)=1
 * Range:     u8 n <= 668, u16 n <= 5000, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This is the recurrence for the partial sums of A189661. */

#include <stddef.h>   /* for NULL */
#include <stdlib.h>   /* for malloc, free */

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;          /* offset is 1, but guard against 0 */

    static seq_t *cache = NULL;    /* cache[0] unused, cache[i] = a(i) */
    static idx_t  cache_size = 0;  /* highest index for which cache is valid */

    /* Allocate or extend the cache to support index n. */
    if (n > cache_size) {
        /* Allocate new storage of size n+1 (indices 0..n). */
        seq_t *new_cache = (seq_t *)malloc(sizeof(seq_t) * (n + 1));
        if (!new_cache)
            return (seq_t)0; /* allocation failed – return 0 as fallback */

        /* Copy existing values into the new cache. */
        for (idx_t i = 1; i <= cache_size; ++i)
            new_cache[i] = cache[i];

        /* Free old cache and replace it. */
        free(cache);
        cache = new_cache;

        /* Compute new terms in increasing order, so all dependencies are ready. */
        for (idx_t i = cache_size + 1; i <= n; ++i) {
            if (i == 1)
                cache[1] = (seq_t)0;
            else if (i == 2)
                cache[2] = (seq_t)1;
            else {
                idx_t k = (idx_t)cache[i - 2];
                /* 1 + k and (i-1) - k are guaranteed to be valid indices ≤ i-1. */
                cache[i] = cache[1 + k] + cache[(i - 1) - k];
            }
        }

        cache_size = n;
    }

    return cache[n];
}
