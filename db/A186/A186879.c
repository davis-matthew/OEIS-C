/*
 * A186879  Number of (n+2)X9 0..3 arrays with every 3X3 subblock commuting
 *          with each horizontal and vertical neighbor 3X3 subblock.
 * https://oeis.org/A186879
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) =
 *            2*a(n-1)-a(n-2)+3*a(n-3)-6*a(n-4)+3*a(n-5)+3*a(n-6)-3*a(n-7) for
 *            n>15 /* Initial values (n = 1..15):
 *            277659,21661,37576,121144,260584,338176,443560,571984,
 *            798832,1272712,2086288,3350152,5585728,9225664,15403771
 * Range:     u8 none, u16 none, u32 n <= 26, u64 n <= 69, u128 n <= 156,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n == 0)  /* Undefined for n==0; return 0 for safety. */
        return (seq_t)0;

    /* Initial 15 terms. */
    static const seq_t init[15] = {
        277659ULL, 21661ULL, 37576ULL, 121144ULL, 260584ULL,
        338176ULL, 443560ULL, 571984ULL, 798832ULL, 1272712ULL,
        2086288ULL, 3350152ULL, 5585728ULL, 9225664ULL, 15403771ULL
    };

    /* Static cache for memoisation.  cache[i] holds a(i+1). */
    static seq_t *cache = NULL;
    static idx_t cached_size = 0;   /* Number of values stored in cache. */
    static idx_t cache_capacity = 0;/* Allocated size of cache.   */

    /* If n is within the initial table, return the constant. */
    if (n <= 15)
        return init[n - 1];

    /* Initialise cache on first use. */
    if (cached_size == 0) {
        cache_capacity = 15;
        cache = (seq_t *)malloc(cache_capacity * sizeof(seq_t));
        if (!cache) return (seq_t)0;
        for (idx_t i = 0; i < 15; ++i)
            cache[i] = init[i];
        cached_size = 15;
    }

    /* Make sure we have space for up to n. */
    if (n > cache_capacity) {
        idx_t new_cap = n;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return (seq_t)0;
        cache = tmp;
        cache_capacity = new_cap;
    }

    /* Compute missing terms using the linear recurrence. */
    for (idx_t k = cached_size + 1; k <= n; ++k) {
        /* Indices: cache[k-2] = a(k-1), cache[k-3] = a(k-2), ...
           cache[k-8] = a(k-7) (defined for k >= 16). */
        seq_t ak = (seq_t)2 * cache[k - 2]
                 - cache[k - 3]
                 + (seq_t)3 * cache[k - 4]
                 - (seq_t)6 * cache[k - 5]
                 + (seq_t)3 * cache[k - 6]
                 + (seq_t)3 * cache[k - 7]
                 - (seq_t)3 * cache[k - 8];
        cache[k - 1] = ak;
    }
    cached_size = n;

    return cache[n - 1];
}
