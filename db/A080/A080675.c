/*
 * A080675  a(n) = (5*4^n - 8)/6.
 * https://oeis.org/A080675
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (5*4^n - 8)/6
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 16, u64 n <= 32, u128 n <= 64,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence (used for computation): a(1)=2, a(2)=12, a(n)=5*a(n-1)-4*a(n-2) for n>=3 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)2;
    if (n == 2) return (seq_t)12;

    /* static cache holds values a(1)...a(cache_len)  (0‑based index: cache[i-1] = a(i)) */
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;        /* number of values currently stored */
    static idx_t cache_cap = 0;        /* memory capacity in elements */

    /* enlarge cache if necessary */
    if (n > cache_len) {
        /* start with capacity 4 if empty */
        if (cache_cap == 0) {
            cache_cap = 4;
            cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
            if (!cache) abort();       /* allocation failure - terminate */
        }
        /* increase capacity until it fits 'n' */
        while (cache_cap < n) {
            cache_cap <<= 1;
            seq_t *tmp = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
            if (!tmp) abort();        /* allocation failure */
            cache = tmp;
        }

        /* initialise the first two terms if cache was empty */
        if (cache_len == 0) {
            cache[0] = (seq_t)2;      /* a(1) */
            cache_len = 1;
        }
        if (cache_len == 1) {
            cache[1] = (seq_t)12;     /* a(2) */
            cache_len = 2;
        }

        /* compute missing terms using the recurrence */
        for (idx_t i = cache_len + 1; i <= n; ++i) {
            idx_t idx = i - 1;        /* zero‑based index for cache */
            /* a(i) = 5*a(i-1) - 4*a(i-2) */
            cache[idx] = 5 * cache[idx - 1] - 4 * cache[idx - 2];
        }
        cache_len = n;
    }

    return cache[n - 1];
}
