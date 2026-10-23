/*
 * A144430  a(n) = 1 + A144429(n).
 * https://oeis.org/A144430
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, less
 * Formula:   a(n) = a(n-3) + 6 for n > 5; a(1) = 2, a(2) = 3, a(3) = 4, a(4)
 *            = 5, a(5) = 7
 * Range:     u8 n <= 128, u16 n <= 2000, u32 n <= 2000, u64 n <= 2000,
 *            u128 n <= 2000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

/* static cache for memoization */
static seq_t *cache = NULL;
static size_t cache_cap = 0;      /* allocated slots (at least 1) */
static idx_t  max_cached = 0;     /* highest index already computed (0 if none) */

seq_t a(idx_t n)
{
    /* The sequence is defined for n ≥ 1.  For n < 1 we return 0. */
    if (n < 1) return (seq_t)0;

    /* If the requested term is already in the cache, return it. */
    if (n <= (idx_t)max_cached)
        return cache[n];

    /* Ensure the cache has space for index n.  Grow by powers of two. */
    size_t new_cap = cache_cap;
    if (new_cap == 0) new_cap = 16;
    while (new_cap <= (size_t)n) new_cap <<= 1;

    seq_t *new_cache = (seq_t *)malloc(new_cap * sizeof(seq_t));
    if (!new_cache) return (seq_t)0;   /* allocation failed */

    /* Copy existing cached values to the new array. */
    if (cache) {
        for (idx_t i = 1; i <= max_cached; ++i)
            new_cache[i] = cache[i];
        new_cache[0] = (seq_t)0;
        free(cache);
    } else {
        new_cache[0] = (seq_t)0;     /* unused slot for convenience */
    }

    cache = new_cache;
    cache_cap = new_cap;

    /* Compute terms up to n using the recurrence. */
    for (idx_t i = max_cached + 1; i <= n; ++i) {
        if (i == 1)       cache[i] = (seq_t)2;
        else if (i == 2)  cache[i] = (seq_t)3;
        else if (i == 3)  cache[i] = (seq_t)4;
        else if (i == 4)  cache[i] = (seq_t)5;
        else if (i == 5)  cache[i] = (seq_t)7;
        else              cache[i] = cache[i - 3] + (seq_t)6;
    }
    max_cached = n;
    return cache[n];
}
