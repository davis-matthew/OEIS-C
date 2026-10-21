/*
 * A235191  Number of (n+1) X (1+1) 0..6 arrays with every 2 X 2 subblock
 *          having its diagonal sum differing from its antidiagonal sum by 5,
 *          with no adjacent elements equal (constant-stress tilted 1 X 1
 *          tilings).
 * https://oeis.org/A235191
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 30*a(n-2) + 468*a(n-6) - 257*a(n-4) for n>=7 Initial
 *            values: a(1) = 136 a(2) = 512 a(3) = 1880 a(4) = 7072 a(5) =
 *            26040 a(6) = 98080 The sequence is computed by a linear
 *            recurrence, memoised in a static array that grows on demand. All
 *            arithmetic uses the nominal integer type seq_t; interme...
 * Range:     u8 n <= 1, u16 n <= 5, u32 n <= 14, u64 n <= 30, u128 n <= 64,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* No result defined for n<1 */
    if (n < 1) return (seq_t)0;

    /* Static cache storing computed values, index 0 unused  */
    static seq_t *cache = NULL;
    static idx_t cache_alloc = 0;   /* allocated size of cache (>= n+1) */
    static idx_t cache_last = 0;    /* largest n whose value is stored */

    /* Initial values for n=1..6 */
    static const seq_t init[7] = {
        0,          /* unused  */
        136,        /* a(1) */
        512,        /* a(2) */
        1880,       /* a(3) */
        7072,       /* a(4) */
        26040,      /* a(5) */
        98080       /* a(6) */
    };

    /* First call: allocate and initialise the cache */
    if (cache == NULL) {
        cache_alloc = 7;                   /* indices 0..6 */
        cache = (seq_t *)malloc((size_t)cache_alloc * sizeof(seq_t));
        if (!cache) exit(EXIT_FAILURE);
        for (idx_t i = 1; i <= 6; ++i)
            cache[i] = init[i];
        cache[0] = 0;                      /* unused */
        cache_last = 6;
    }

    /* If we already have the requested value, return it */
    if (n <= cache_last)
        return cache[n];

    /* Ensure the cache can hold index n */
    if (cache_alloc <= n) {
        idx_t new_alloc = n + 1;           /* allocate up to index n */
        seq_t *tmp = (seq_t *)realloc(cache, (size_t)new_alloc * sizeof(seq_t));
        if (!tmp) exit(EXIT_FAILURE);
        /* initialise new slots to zero */
        for (idx_t i = cache_alloc; i <= n; ++i)
            tmp[i] = (seq_t)0;
        cache = tmp;
        cache_alloc = new_alloc;
    }

    /* Compute missing values from cache_last+1 up to n */
    for (idx_t i = cache_last + 1; i <= n; ++i) {
        if (i <= 6) {                     /* safety guard (should not hit) */
            cache[i] = init[i];
            continue;
        }
        /* a(i) = 30*a(i-2) + 468*a(i-6) - 257*a(i-4) */
        seq_t term1 = (seq_t)30  * cache[i - 2];
        seq_t term2 = (seq_t)468 * cache[i - 6];
        seq_t sub   = (seq_t)257 * cache[i - 4];
        cache[i] = term1 + term2 - sub;
    }
    cache_last = n;
    return cache[n];
}
