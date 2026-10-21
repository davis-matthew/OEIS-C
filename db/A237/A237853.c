/*
 * A237853  Number of (n+1) X (2+1) 0..1 arrays with no element greater than
 *          all horizontal neighbors or less than all vertical neighbors.
 * https://oeis.org/A237853
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 3*a(n-1) - 2*a(n-2) + 4*a(n-3) + 2*a(n-4) + a(n-5) with
 *            initial values a(1)=4, a(2)=14, a(3)=41, a(4)=114, a(5)=325
 *            (empirical recurrence of order 5)
 * Range:     u8 n <= 4, u16 n <= 10, u32 n <= 20, u64 n <= 41, u128 n <= 83,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache for memoised values */
    static seq_t *cache = NULL;
    static size_t cap   = 0;      /* allocated size of cache (>= max_n+1) */
    static idx_t  max_n = 0;      /* highest n already computed */

    /* initialise cache with known values (indices 1..5) */
    if (n <= 5) {
        switch (n) {
            case 1: return (seq_t)4;
            case 2: return (seq_t)14;
            case 3: return (seq_t)41;
            case 4: return (seq_t)114;
            case 5: return (seq_t)325;
            default: return (seq_t)0; /* protects n==0, though offset=1 */
        }
    }

    if (cache == NULL) {
        /* start with some capacity: at least 6, then grow */
        cap   = 64;
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
        if (cache == NULL) abort();  /* out of memory */

        /* initialise known values */
        cache[1] = (seq_t)4;
        cache[2] = (seq_t)14;
        cache[3] = (seq_t)41;
        cache[4] = (seq_t)114;
        cache[5] = (seq_t)325;
        max_n = 5;
    }

    if (n + 1 > cap) {
        /* grow capacity (doubling strategy) */
        size_t new_cap = cap * 2;
        if (new_cap < n + 1) new_cap = n + 1;
        seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (new_cache == NULL) abort();
        cache = new_cache;
        cap   = new_cap;
    }

    /* compute missing terms up to n */
    for (idx_t i = max_n + 1; i <= n; ++i) {
        seq_t val = 3 * cache[i-1]
                  - 2 * cache[i-2]
                  + 4 * cache[i-3]
                  + 2 * cache[i-4]
                  +     cache[i-5];
        cache[i] = val;
    }
    max_n = n;
    return cache[n];
}
