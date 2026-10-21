/*
 * A092500  a(0)=0; for n>0, a(n) = a(n-1)^2 + 10.
 * https://oeis.org/A092500
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 0; for n > 0, a(n) = a(n-1)^2 + 10
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 4, u64 n <= 5, u128 n <= 6,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache for previously computed terms */
    static seq_t *cache = NULL;      /* array holding a(0)..a(cache_last) */
    static size_t cache_cap = 0;     /* allocated capacity (in elements) */
    static idx_t cache_last = (idx_t)-1; /* highest n for which cache[n] is valid;
                                            -1 means no values computed yet */

    /* ensure the allocation covers n */
    if (cache_cap < (size_t)(n + 1)) {
        size_t new_cap = (size_t)(n + 1);
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp)          /* allocation failure – return 0 as a harmless fallback */
            return (seq_t)0;
        cache = tmp;
        cache_cap = new_cap;
    }

    /* initialise the base case a(0) = 0 on first use */
    if (cache_last == (idx_t)-1) {
        cache[0] = (seq_t)0;
        cache_last = 0;
    }

    /* already computed? */
    if (n <= cache_last)
        return cache[n];

    /* compute missing terms up to n */
    for (idx_t i = cache_last + 1; i <= n; ++i)
        cache[i] = cache[i - 1] * cache[i - 1] + (seq_t)10;

    cache_last = n;
    return cache[n];
}
