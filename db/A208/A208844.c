/*
 * A208844  Number of 7 X n 0..1 arrays avoiding 0 0 0 and 0 0 1 horizontally
 *          and 0 1 1 and 1 1 0 vertically.
 * https://oeis.org/A208844
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*a(n-1) + 33*a(n-2) - 32*a(n-3); Initial values: a(1) =
 *            35, a(2) = 1225, a(3) = 2485
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 10, u64 n <= 22, u128 n <= 46,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* Static memoisation cache: cache[i] contains a(i) for i >= 1. */
    static seq_t *cache = NULL;      /* pointer to the array */
    static idx_t  cache_len = 0;     /* largest index stored */
    static idx_t  cache_cap = 0;     /* capacity of the array (max index) */

    /* On first call, allocate space for indices 0..3 and initialise. */
    if (cache == NULL)
    {
        cache_cap = 4;                                   /* capacity for indices 0..4 */
        cache = (seq_t*)malloc(sizeof(seq_t) * (cache_cap + 1));
        if (cache == NULL) exit(1); /* allocation failed */
        for (idx_t i = 0; i <= cache_cap; ++i) cache[i] = 0;   /* zero‑initialise */
        cache[1] = (seq_t)35;
        cache[2] = (seq_t)1225;
        cache[3] = (seq_t)2485;
        cache_len = 3;
    }

    /* If the requested index is already cached, return it. */
    if ((idx_t)n <= cache_len)
        return cache[n];

    /* Expand the cache if necessary. */
    if ((idx_t)n > cache_cap)
    {
        idx_t new_cap = cache_cap;
        while (new_cap < (idx_t)n)
            new_cap <<= 1;           /* double until sufficient */
        seq_t *tmp = (seq_t*)realloc(cache, sizeof(seq_t) * (new_cap + 1));
        if (tmp == NULL) exit(1);    /* allocation failed */
        cache = tmp;
        for (idx_t i = cache_cap + 1; i <= new_cap; ++i)
            cache[i] = 0;            /* initialise new entries */
        cache_cap = new_cap;
    }

    /* Compute missing terms up to n using the recurrence. */
    for (idx_t i = cache_len + 1; i <= n; ++i)
    {
        cache[i] = 2 * cache[i - 1] + 33 * cache[i - 2] - 32 * cache[i - 3];
    }
    cache_len = n;

    return cache[n];
}
