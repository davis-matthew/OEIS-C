/*
 * A084261  A binomial transform of factorial numbers.
 * https://oeis.org/A084261
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 12, u32 n <= 20, u64 n <= 36, u128 n <= 62,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0)=1, a(1)=1, a(2)=2,
   a(n) = (4*a(n-1)+(n-2)*a(n-2)-(n-2)*a(n-3))/2   for n>=3
   (derived from 2*a(n)‑4*a(n-1)+(-n+2)*a(n-2)+(n-2)*a(n-3)=0) */

#include <stdlib.h>

seq_t a(idx_t n) {
    /* lazily grow a static cache of computed terms */
    static seq_t *cache = NULL;
    static idx_t cache_cap = 0;   /* allocated size */
    static idx_t cache_len = 0;   /* number of terms stored (max index + 1) */

    /* initialise cache for indices 0,1,2 */
    if (!cache) {
        cache_cap = 32;                      /* initial capacity */
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache) return (seq_t)0;         /* allocation failure (unlikely) */
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)1;
        cache[2] = (seq_t)2;
        cache_len = 3;
    }

    /* if requested index is already in cache */
    if (n < cache_len)
        return cache[n];

    /* grow cache to accommodate up to n */
    if (n >= cache_cap) {
        idx_t new_cap = cache_cap;
        while (new_cap <= n) new_cap <<= 1;
        seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!new_cache) return (seq_t)0;     /* allocation failure */
        cache = new_cache;
        cache_cap = new_cap;
    }

    /* compute missing terms using the recurrence */
    for (idx_t i = cache_len; i <= n; ++i) {
        /* compute 4*a[i-1] + (i-2)*a[i-2] - (i-2)*a[i-3] */
        seq_t term1 = cache[i-1] * (seq_t)4;
        seq_t term2 = cache[i-2] * (seq_t)(i - 2);
        seq_t sum   = term1 + term2;
        sum -= cache[i-3] * (seq_t)(i - 2);   /* subtract (i-2)*a[i-3] */
        cache[i] = sum / (seq_t)2;             /* integer division (even) */
    }
    cache_len = n + 1;  /* update stored length */

    return cache[n];
}
