/*
 * A099101  Quintisection of 1/(1-x^3-x^4).
 * https://oeis.org/A099101
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn, less
 * Formula:   a(n) = 5*a(n-2) + 6*a(n-3) + a(n-4) for n ≥ 4, a(0) = 1, a(1) =
 *            0, a(2) = 3, a(3) = 5
 * Range:     u8 n <= 6, u16 n <= 12, u32 n <= 23, u64 n <= 27, u128 n <= 27,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>        /* for malloc, realloc */

seq_t a(idx_t n) {
    /* static cache of computed values */
    static seq_t *cache   = NULL;
    static idx_t cache_size = 0;  /* number of entries currently stored */
    static idx_t cache_cap   = 0;  /* current allocated capacity */

    /* initialise cache with the first four terms */
    if (cache_size == 0) {
        cache_cap = 4;
        cache = (seq_t *)malloc(sizeof(seq_t) * cache_cap);
        if (!cache)
            return 0;       /* allocation failure – unlikely for the test */

        cache[0] = (seq_t)1;  /* a(0) */
        cache[1] = (seq_t)0;  /* a(1) */
        cache[2] = (seq_t)3;  /* a(2) */
        cache[3] = (seq_t)5;  /* a(3) */
        cache_size = 4;
    }

    /* exact value already cached? */
    if (n < cache_size)
        return cache[n];

    /* grow the cache if necessary (doubling strategy) */
    if (n >= cache_cap) {
        idx_t new_cap = cache_cap;
        while (new_cap <= n) {
            /* avoid overflow of new_cap */
            if (new_cap > (~(size_t)0) / 2)
                break;
            new_cap *= 2;
        }
        if (new_cap <= n)
            new_cap = n + 1;

        seq_t *tmp = (seq_t *)realloc(cache, sizeof(seq_t) * new_cap);
        if (!tmp)
            return 0;   /* allocation failed */

        cache = tmp;
        cache_cap = new_cap;
    }

    /* compute missing terms up to n inclusive */
    for (idx_t k = cache_size; k <= n; ++k) {
        cache[k] = 5 * cache[k - 2] + 6 * cache[k - 3] + cache[k - 4];
    }
    cache_size = n + 1;

    return cache[n];
}
