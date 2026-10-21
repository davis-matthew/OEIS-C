/*
 * A052501  Number of permutations sigma such that sigma^5=Id; degree-n
 *          permutations of order dividing 5.
 * https://oeis.org/A052501
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, nice, easy
 * Formula:   a(n) = a(n-1) + a(n-5)*(n-1)*(n-2)*(n-3)*(n-4) for n>=5 with
 *            a(0)=a(1)=a(2)=a(3)=a(4)=1
 * Range:     u8 n <= 6, u16 n <= 9, u32 n <= 15, u64 n <= 24, u128 n <= 40,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache for computed values */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;   /* size of allocated cache */
    static idx_t cache_max = (idx_t)-1;  /* highest n with a computed value */

    /* lazy allocation or resize to provide n+1 slots */
    if (!cache) {
        cache_cap = (size_t)(n + 1 > 5 ? n + 1 : 5);
        cache = (seq_t*)malloc(cache_cap * sizeof(seq_t));
        if (!cache) return (seq_t)0;            /* allocation failure */
        for (idx_t i = 0; i < 5 && i < cache_cap; ++i)
            cache[i] = (seq_t)1;                /* a(0)..a(4) = 1 */
        for (idx_t i = 5; i < cache_cap; ++i)
            cache[i] = (seq_t)0;                /* remaining elements zeroed */
        cache_max = 4;
    } else if ((size_t)n + 1 > cache_cap) {
        size_t new_cap = cache_cap ? cache_cap : 5;
        while (new_cap <= (size_t)n) new_cap <<= 1;
        seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return (seq_t)0;              /* allocation failure */
        cache = tmp;
        for (idx_t i = cache_cap; i <= (idx_t)n; ++i)
            cache[i] = (seq_t)0;
        cache_cap = new_cap;
    }

    /* if required value already computed, return it */
    if (cache_max >= n) return cache[n];

    /* compute values up to n using the recurrence */
    for (idx_t i = cache_max + 1; i <= n; ++i) {
        seq_t prod = (seq_t)(i - 1);
        prod *= (seq_t)(i - 2);
        prod *= (seq_t)(i - 3);
        prod *= (seq_t)(i - 4);
        cache[i] = cache[i - 1] + cache[i - 5] * prod;
    }
    cache_max = n;

    return cache[n];
}
