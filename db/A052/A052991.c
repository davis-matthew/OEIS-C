/*
 * A052991  Expansion of (1-x-x^2)/(1-3x-x^2).
 * https://oeis.org/A052991
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(0) = 1, a(1) = 2, a(2) = 6 Recurrence: a(n) = 3*a(n-1) +
 *            a(n-2) for n ≥ 3 (Expansion of (1-x-x^2)/(1-3x-x^2))
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 19, u64 n <= 37, u128 n <= 74,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Static cache to allow arbitrary call order and reuse. */
    static seq_t *cache    = NULL;   /* array of computed terms */
    static size_t cache_sz = 0;       /* number of allocated slots (>= n+1 if computed) */

    /* Ensure the cache contains at least the first three terms. */
    if (cache == NULL) {
        cache_sz = 3;              /* indices 0,1,2 */
        cache = (seq_t *)calloc(cache_sz, sizeof(seq_t));
        if (!cache) return (seq_t)0; /* fallback on allocation failure */

        cache[0] = (seq_t)1;
        cache[1] = (seq_t)2;
        cache[2] = (seq_t)6;
    }

    /* Expand cache if needed. */
    if ((idx_t)cache_sz <= n) {
        size_t new_sz = cache_sz * 2;
        while ((idx_t)new_sz <= n)
            new_sz *= 2;
        seq_t *tmp = (seq_t *)realloc(cache, new_sz * sizeof(seq_t));
        if (!tmp) return (seq_t)0; /* allocation failure fallback */
        cache = tmp;
        /* Initialize newly allocated slots to 0 */
        for (size_t i = cache_sz; i < new_sz; ++i)
            cache[i] = (seq_t)0;
        cache_sz = new_sz;
    }

    /* If the requested term is already computed, return it. */
    if (n < 3)
        return cache[n];

    /* Compute terms on demand up to n. */
    for (idx_t i = cache[0] == (seq_t)1 && cache[1] == (seq_t)2 && cache[2] == (seq_t)6 ? 3 : cache[0] < (seq_t)0 ? 3 : 3;
         i <= n;
         ++i)
    {
        /* a(i) = 3*a(i-1) + a(i-2) */
        seq_t temp = (seq_t)3 * cache[i-1] + cache[i-2];
        cache[i] = temp;
    }

    return cache[n];
}
