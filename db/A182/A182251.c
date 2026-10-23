/*
 * A182251  a(0) = 0, a(n) = (a(n-1) OR n) * n.
 * https://oeis.org/A182251
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(0) = 0 a(n) = (a(n-1) | n) * n for n ≥ 1 (bitwise
 *            inclusive-OR)
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 12, u64 n <= 20, u128 n <= 33,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    /* Return the base case directly */
    if (n == 0)
        return (seq_t)0;

    /* Static cache for previously computed values */
    static seq_t *cache = NULL;   /* array of seq_t values */
    static size_t cap   = 0;      /* capacity of cache */
    static idx_t  last  = 0;      /* highest index currently stored */

    /* Initialise cache on first use */
    if (cache == NULL) {
        size_t needed = (size_t)n + 1;   /* at least (n+1) elements */
        if (needed == 0)                /* guard against overflow when n == SIZE_MAX */
            needed = 1;
        cache = (seq_t *)malloc(needed * sizeof(seq_t));
        if (!cache) abort();            /* out‑of‑memory */
        cache[0] = (seq_t)0;            /* a(0) */
        cap   = needed;
        last  = 0;
    }
    else if ((size_t)(n + 1) > cap) {
        /* Grow cache exponentially to keep amortised cost O(1) per new element */
        size_t newcap = cap;
        while ((size_t)(n + 1) > newcap) newcap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, newcap * sizeof(seq_t));
        if (!tmp) abort();
        cache = tmp;
        cap   = newcap;
    }

    /* Compute missing terms up to n (inclusive) */
    for (idx_t i = last + 1; i <= n; ++i) {
        seq_t prev = cache[i - 1];
        seq_t val  = (prev | (seq_t)i) * (seq_t)i;
        cache[i] = val;
    }
    last = n;

    return cache[n];
}
