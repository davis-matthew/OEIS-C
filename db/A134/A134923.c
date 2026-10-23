/*
 * A134923  a(n) = (a(n-1)+a(n-2))^3, for n>=2; a(0)=0,a(1)=1.
 * https://oeis.org/A134923
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (a(n-1)+a(n-2))^3, a(0)=0, a(1)=1
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 5, u64 n <= 5, u128 n <= 6,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache for previously computed values */
    static seq_t *cache      = NULL;
    static size_t cache_cap  = 0;   /* capacity of the cache array */
    static idx_t  computed_last = 1; /* last index whose value is known */

    /* initialise cache with base cases */
    if (cache_cap == 0) {
        cache_cap = 2; /* store indices 0 and 1 */
#ifdef __cplusplus
        cache = new seq_t[cache_cap];
#else
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
#endif
        if (!cache) return (seq_t)0;   /* allocation failure => zero */
        cache[0] = (seq_t)0;
        cache[1] = (seq_t)1;
    }

    /* trivial cases */
    if (n == 0) return cache[0];
    if (n == 1) return cache[1];

    /* grow cache if necessary */
    if ((size_t)n + 1 > cache_cap) {
        size_t new_cap = cache_cap;
        while ((size_t)n + 1 > new_cap) new_cap *= 2;
#ifdef __cplusplus
        seq_t *tmp = new seq_t[new_cap];
        /* copy existing values */
        for (size_t i = 0; i < cache_cap; ++i) tmp[i] = cache[i];
        delete [] cache;
        cache = tmp;
#else
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        /* on failure keep old cache and return zero */
        if (!tmp) return (seq_t)0;
        cache = tmp;
#endif
        cache_cap = new_cap;
    }

    /* compute missing values up to n */
    for (idx_t i = computed_last + 1; i <= n; ++i) {
        seq_t s = cache[(size_t)(i - 1)] + cache[(size_t)(i - 2)];
        cache[(size_t)i] = s * s * s;
    }
    computed_last = n;

    return cache[(size_t)n];
}
