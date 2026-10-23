/*
 * A123887  Expansion of g.f.: (1+x+x^2)/(1-5*x-5*x^2).
 * https://oeis.org/A123887
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0)=1, a(1)=6, a(2)=36, a(n)=5*a(n-1)+5*a(n-2) for n>2
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 12, u64 n <= 25, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Static cache to memoise computed terms */
    static seq_t *cache = NULL;
    static size_t cache_size = 0;   /* number of allocated slots */
    static idx_t max_comp = (idx_t)-1; /* highest index with a computed value */

    /* Direct return for first three terms */
    if (n <= 2) {
        switch ((int)n) {
        case 0: return 1;
        case 1: return 6;
        case 2: return 36;
        }
    }

    /* Ensure the cache can hold up to index n */
    if (cache == NULL || cache_size <= n) {
        size_t new_size = (size_t)(n + 1);
        if (cache == NULL) {
            cache = (seq_t *)malloc(new_size * sizeof(seq_t));
            if (!cache) abort();          /* allocation failure */
            /* initialise known terms */
            cache[0] = 1;
            cache[1] = 6;
            cache[2] = 36;
            cache_size = new_size;
            max_comp   = 2;
        } else {
            /* enlarge existing buffer */
            seq_t *tmp = (seq_t *)realloc((void *)cache,
                                          new_size * sizeof(seq_t));
            if (!tmp) abort();            /* allocation failure */
            cache      = tmp;
            cache_size = new_size;
            /* no need to initialise new slots; they will be computed below */
        }
    }

    /* Compute any missing terms up to n */
    for (idx_t i = max_comp + 1; i <= n; ++i) {
        cache[i] = (seq_t)5 * cache[i - 1] + (seq_t)5 * cache[i - 2];
    }
    max_comp = n;
    return cache[n];
}
