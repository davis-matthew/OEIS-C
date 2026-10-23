/*
 * A015544  Lucas sequence U(5,-8): a(n+1) = 5*a(n) + 8*a(n-1), a(0)=0,
 *          a(1)=1.
 * https://oeis.org/A015544
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 5*a(n-1) + 8*a(n-2), with a(0)=0, a(1)=1
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 13, u64 n <= 25, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

/* Compute sequence A015544 (Lucas sequence U(5,-8)). */
seq_t a(idx_t n)
{
    /* static cache and meta‑data */
    static seq_t *cache = NULL;
    static size_t  cache_cap = 0;
    static idx_t   max_comp = 0;

    /* First call: allocate space and initialise a[0] and a[1]. */
    if (cache == NULL)
    {
        cache_cap = (size_t)2;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (cache == NULL)      /* allocation failed – return 0 as a safe fallback */
            return (seq_t)0;
        cache[0] = (seq_t)0;
        cache[1] = (seq_t)1;
        max_comp = 1;
    }

    /* Return cached value if already computed. */
    if (n <= max_comp)
        return cache[(size_t)n];

    /* Grow the cache to hold indices up to n. */
    size_t needed = (size_t)(n + 1);
    if (needed > cache_cap)
    {
        seq_t *tmp = (seq_t *)realloc(cache, needed * sizeof(seq_t));
        if (tmp == NULL)
            return (seq_t)0;                   /* allocation failed – return 0 */
        cache = tmp;
        cache_cap = needed;
    }

    /* Compute missing terms using the recurrence. */
    for (idx_t i = max_comp + 1; i <= n; ++i)
    {
        size_t k = (size_t)i;
        cache[k] = (seq_t)5 * cache[k - 1] + (seq_t)8 * cache[k - 2];
    }
    max_comp = n;
    return cache[(size_t)n];
}
