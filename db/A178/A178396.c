/*
 * A178396  a(1)=1, a(n) = a(n-1)*1000 + 11^(n-1) for n>=2.
 * https://oeis.org/A178396
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1) * 1000 + 11^(n-1), a(1) = 1
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 4, u64 n <= 7, u128 n <= 13,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;          /* memoisation array */
    static size_t cache_cap = 0;         /* current allocation size   */
    static idx_t  max_cached = 0;        /* largest n that has been computed      */
    static seq_t  power_11_current = 0;  /* 11^(max_cached-1)   */

    if (n == 0) return 0;   /* not used, but keeps the function safe */

    /* already computed? */
    if (n <= max_cached) {
        return cache[n-1];
    }

    /* Ensure the cache is large enough to store up to n */
    size_t needed = (size_t)n;
    if (cache == NULL) {
        size_t init = needed > 1 ? needed : 1;
        cache = (seq_t*)malloc(sizeof(seq_t) * init);
        if (!cache) return 0;   /* allocation failure */
        cache_cap = init;
    } else if (needed > cache_cap) {
        size_t new_cap = cache_cap;
        while (new_cap < needed) new_cap *= 2;
        seq_t *tmp = (seq_t*)realloc(cache, sizeof(seq_t) * new_cap);
        if (!tmp) return 0;   /* allocation failure */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Initialise first element if this is the first call */
    if (max_cached == 0) {
        cache[0] = 1;                /* a(1) = 1 */
        power_11_current = 1;        /* 11^(1-1) */
        max_cached = 1;
    }

    /* Compute missing terms up to n */
    for (idx_t i = max_cached + 1; i <= n; ++i) {
        power_11_current = power_11_current * ((seq_t)11);
        cache[i-1] = cache[i-2] * ((seq_t)1000) + power_11_current;
    }
    max_cached = n;
    return cache[n-1];
}
