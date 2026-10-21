/*
 * A095897  Expansion of x*(1+4*x-4*x^2)/((1+2*x)*(1-6*x)*(1-8*x^2)).
 * https://oeis.org/A095897
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 13, u64 n <= 25, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Recurrence:
 *   a(1) = 1, a(2) = 8, a(3) = 48, a(4) = 320;
 *   a(n) = 4*a(n-1) + 20*a(n-2) - 32*a(n-3) - 96*a(n-4)  for n > 4.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* cache previously computed terms for subsequent calls */
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;   /* largest computed index */
    static idx_t cache_cap  = 0;   /* allocated capacity (>= cache_size+1) */
    idx_t i;

    if (n < 1) {
        /* offset = 1, so values for n < 1 are not defined */
        return (seq_t)0;
    }

    /* Ensure capacity for index n */
    if (cache_cap < n + 1) {
        idx_t new_cap = cache_cap ? cache_cap * 2 : 32;
        if (new_cap < n + 1) new_cap = n + 1;
        seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!new_cache) {   /* allocation failure: return 0 as a fallback */
            return (seq_t)0;
        }
        cache = new_cache;
        cache_cap = new_cap;
    }

    /* Initialize first four terms if not already done */
    if (cache_size < 5) {
        if (cache_size < 1) { cache[1] = (seq_t)1;   cache_size = 1; }
        if (cache_size < 2) { cache[2] = (seq_t)8;   cache_size = 2; }
        if (cache_size < 3) { cache[3] = (seq_t)48;  cache_size = 3; }
        if (cache_size < 4) { cache[4] = (seq_t)320; cache_size = 4; }
    }

    /* Compute missing terms up to n */
    for (i = cache_size + 1; i <= n; ++i) {
        seq_t term = (seq_t)4 * cache[i - 1] + (seq_t)20 * cache[i - 2];
        term -= ( (seq_t)32 * cache[i - 3] + (seq_t)96 * cache[i - 4] );
        cache[i] = term;
    }
    cache_size = n;   /* now we have computed up to n */

    return cache[n];
}
