/*
 * A083425  a(n) = (5*5^n + (-1)^n)/6.
 * https://oeis.org/A083425
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (5*5^n + (-1)^n)/6
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 13, u64 n <= 27, u128 n <= 55,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0)=1, a(1)=4, a(n)=4*a(n-1)+5*a(n-2) for n≥2 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache for memoisation */
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;   /* number of valid entries in cache */
    static idx_t cache_cap = 0;   /* allocated capacity */
    idx_t i;

    /* initialise cache on first call */
    if (cache_len == 0) {
        cache_cap = (n < 2 ? 2 : n * 2);
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache) return 0;        /* allocation failure – abort */
        cache[0] = 1;               /* a(0) */
        cache_len = 1;
        if (n >= 1) {
            cache[1] = 4;           /* a(1) */
            ++cache_len;
        }
    } else if (n >= cache_len) {   /* cache too small – grow */
        idx_t new_cap = n + 1;
        if (new_cap > cache_cap) {
            cache_cap = new_cap;
            cache = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
            if (!cache) return 0;  /* allocation failure – abort */
        }
        if (cache_len == 1 && n >= 1) {
            cache[1] = 4;           /* a(1) */
            ++cache_len;
        }
    }

    /* compute missing terms using the recurrence */
    for (i = cache_len; i <= n; ++i) {
        cache[i] = 4 * cache[i-1] + 5 * cache[i-2];
    }
    cache_len = n + 1;  /* all indices [0..n] are filled */

    return cache[n];
}
