/*
 * A153709  Expansion of (1 + 7*x)/(1 - 11*x - 26*x^2).
 * https://oeis.org/A153709
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0)=1, a(1)=18, a(n)=11*a(n-1)+26*a(n-2) for n>=2
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 17, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache    = NULL;   /* cached values */
    static idx_t cache_len = 0;      /* number of stored entries */

    /* Return cached value if already computed */
    if (n < cache_len)
        return cache[n];

    /* Need to compute up to n */
    idx_t new_len = n + 1;           /* number of entries required */

    if (!cache) {
        /* First allocation */
        cache = (seq_t *)malloc((size_t)new_len * sizeof(seq_t));
        if (!cache)   /* allocation failure */
            return (seq_t)0;
        cache_len = new_len;

        /* Initialise the first terms */
        cache[0] = (seq_t)1;
        if (new_len > 1)
            cache[1] = (seq_t)18;

        /* Compute remaining terms */
        for (idx_t i = 2; i <= n; ++i)
            cache[i] = ((seq_t)11 * cache[i-1]) + ((seq_t)26 * cache[i-2]);

        return cache[n];
    }

    /* Existing cache – grow and compute new terms */
    idx_t old_len = cache_len;
    seq_t *tmp = (seq_t *)realloc(cache, (size_t)new_len * sizeof(seq_t));
    if (!tmp) {   /* realloc failed – recoverable fallback */
        free(cache);
        cache = NULL;
        cache_len = 0;
        return (seq_t)0;
    }
    cache = tmp;
    cache_len = new_len;

    /* If we lost cache[1] (call happened after n==0 earlier), initialise it */
    if (old_len < 2 && new_len > 1)
        cache[1] = (seq_t)18;

    /* Compute new terms starting after the previously computed range */
    idx_t start = (old_len > 1) ? old_len : 2;
    for (idx_t i = start; i <= n; ++i)
        cache[i] = ((seq_t)11 * cache[i-1]) + ((seq_t)26 * cache[i-2]);

    return cache[n];
}
