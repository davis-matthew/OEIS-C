/*
 * A168207  a(n) = 5*n - a(n-1) + 1 with n>1, a(1)=2.
 * https://oeis.org/A168207
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 5*n + 6 - a(n-1); a(1) = 2 * (which follows from the
 *            closed form a(n) = (10*n + 17 + 19*(-1)^n)/4)
 * Range:     u8 n <= 99, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;

    /* The sequence is defined for n >= 1. */
    if (n == 0)
        return (seq_t)0;          /* unused, but keeps the function safe */

    /* Initialise cache with the first term if it is empty. */
    if (cache_size == 0) {
        cache     = (seq_t *)malloc(sizeof(seq_t));
        if (cache == NULL)
            abort();               /* malloc failure – propagate an error */
        cache[0]  = (seq_t)2;   /* a(1) */
        cache_size = 1;
        if (n == 1)
            return cache[0];
    }

    /* If the requested term is already cached, return it. */
    if (n <= cache_size)
        return cache[n - 1];

    /* Grow the cache to size n.  This is O(n) and keeps O(n) time overall. */
    seq_t *new_cache = (seq_t *)malloc(sizeof(seq_t) * (size_t)n);
    if (new_cache == NULL)
        abort();                  /* memory allocation failed */

    /* Copy the already computed terms. */
    for (idx_t i = 0; i < cache_size; ++i)
        new_cache[i] = cache[i];

    free(cache);
    cache     = new_cache;

    /* Compute the missing terms using the recurrence. */
    seq_t prev = cache[cache_size - 1];          /* a(cache_size) */
    for (idx_t i = cache_size + 1; i <= n; ++i) {
        seq_t current = (seq_t)5 * (seq_t)i + (seq_t)6 - prev;
        cache[cache_size] = current;
        ++cache_size;
        prev = current;
    }

    return cache[n - 1];
}
