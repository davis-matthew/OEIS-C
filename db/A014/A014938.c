/*
 * A014938  a(1)=1, a(n) = n*21^(n-1) + a(n-1).
 * https://oeis.org/A014938
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (21^n * (20n - 1) + 1) / 400 Recurrence: a(1) = 1, a(2) =
 *            43, a(n) = 42*a(n-1) - 441*a(n-2) + 1 (n > 2)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 7, u64 n <= 14, u128 n <= 29,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Static cache holding computed values, 1‑based indexing.
       cache[0] is unused. */
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;     /* number of valid terms in cache */
    static size_t cache_cap = 0;    /* allocated capacity (including index 0) */

    if (n == 0) return 0;          /* undefined, but keep safe */

    /* Grow cache if necessary */
    if (n + 1 > cache_cap) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 16;
        while (new_cap < (size_t)(n + 1))
            new_cap *= 2;
        seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Initialise base values if cache is empty */
    if (cache_len == 0) {
        cache[1] = 1;
        cache[2] = 43;
        cache_len = 2;
    }

    /* If the requested n is already computed, return it */
    if (n <= cache_len)
        return cache[n];

    /* Compute missing terms using the recurrence */
    for (idx_t i = cache_len + 1; i <= n; ++i) {
        /* a(i) = 42*a(i-1) - 441*a(i-2) + 1  */
        cache[i] = cache[i - 1] * ((seq_t)42)
                 - cache[i - 2] * ((seq_t)441)
                 + ((seq_t)1);
    }
    cache_len = n;
    return cache[n];
}
