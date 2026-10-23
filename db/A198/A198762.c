/*
 * A198762  a(n) = 3*5^n - 1 = 2*A057651(n).
 * https://oeis.org/A198762
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3*5^n - 1 (a(0)=2, a(n)=5*a(n-1)+4)
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 13, u64 n <= 26, u128 n <= 54,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache for memoisation */
    static seq_t *cache   = NULL;   /* array of computed terms */
    static idx_t cache_sz = 0;      /* number of valid entries */
    static size_t cache_cap = 0;    /* allocated capacity */

    /* initialise cache on first call */
    if (cache == NULL) {
        cache_cap = 1;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache) abort();
        cache[0] = (seq_t)2;        /* a(0) = 3*5^0 - 1 = 2 */
        cache_sz = 1;
    }

    /* enlarge capacity if needed (doubling strategy) */
    if (n >= (idx_t)cache_cap) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 1;
        while ((idx_t)new_cap <= n)
            new_cap = new_cap * 2 + 1;  /* grow progressively */
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();
        cache = tmp;
        cache_cap = new_cap;
    }

    /* compute any missing terms up to n */
    for (idx_t i = cache_sz; i <= n; ++i) {
        /* recurrence: a(i) = 5*a(i-1)+4   (valid for i>0) */
        if (i == 0) continue;   /* already set */
        cache[i] = (seq_t)5 * cache[i - 1] + (seq_t)4;
    }
    if (cache_sz <= n)
        cache_sz = n + 1;

    return cache[n];
}
