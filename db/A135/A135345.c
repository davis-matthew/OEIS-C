/*
 * A135345  a(n) = 3*a(n-1) + 4*a(n-2) - a(n-3) + 3*a(n-4) + 4*a(n-5).
 * https://oeis.org/A135345
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3*a(n-1) + 4*a(n-2) - a(n-3) + 3*a(n-4) + 4*a(n-5) , a(0)
 *            = 1, a(1) = 4, a(2) = 13, a(3) = 51, a(4) = 204
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 16, u64 n <= 32, u128 n <= 64,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

static seq_t *cache = NULL;      /* memoised values */
static size_t cache_len = 0;     /* number of valid entries stored */
static size_t cache_cap = 0;     /* allocated capacity */

/* Ensure the cache can hold at least 'min_cap' elements; double capacity
   until it reaches that size. */
static void grow_cache(idx_t min_cap)
{
    if (cache_cap < (size_t)min_cap)
    {
        size_t new_cap = cache_cap ? cache_cap * 2 : 8;
        while (new_cap < (size_t)min_cap)
            new_cap *= 2;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp)
            exit(EXIT_FAILURE);      /* out of memory */
        cache = tmp;
        cache_cap = new_cap;
    }
}

seq_t a(idx_t n)
{
    /* Handle the offset 0 base directly. */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)4;
    if (n == 2) return (seq_t)13;
    if (n == 3) return (seq_t)51;
    if (n == 4) return (seq_t)204;

    /* Initial allocation and fill on first call. */
    if (cache == NULL)
    {
        cache_cap = 8;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache)
            exit(EXIT_FAILURE);
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)4;
        cache[2] = (seq_t)13;
        cache[3] = (seq_t)51;
        cache[4] = (seq_t)204;
        cache_len = 5;
    }

    /* Grow cache if required. */
    if ((size_t)n >= cache_cap)
        grow_cache((size_t)n + 1);

    /* Compute missing terms up to n. */
    for (size_t i = cache_len; i <= (size_t)n; ++i)
    {
        seq_t val = 3 * cache[i-1] + 4 * cache[i-2]
                    - cache[i-3] + 3 * cache[i-4] + 4 * cache[i-5];
        cache[i] = val;
    }
    cache_len = (size_t)n + 1;

    return cache[(size_t)n];
}
