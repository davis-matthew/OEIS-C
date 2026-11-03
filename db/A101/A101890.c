/*
 * A101890  a(n) = Sum_{k=0..floor(n/2)} binomial(n,2*k)*Fibonacci(k).
 * https://oeis.org/A101890
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 4*a(n-1)-5*a(n-2)+2*a(n-3)+a(n-4) Initial values: a(0)=0,
 *            a(1)=0, a(2)=1, a(3)=3
 * Range:     u8 n <= 8, u16 n <= 15, u32 n <= 28, u64 n <= 30, u128 n <= 30,
 *            big any n
 * Validated: partial, pass, 31 terms through n = 30 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include "../utils/mem.h"

/* helper to allocate an array of seq_t that works in both C and C++ */
static inline seq_t *seq_malloc(idx_t n)
{
#ifdef __cplusplus
    return mem_new(seq_t, n);
#else
    return (seq_t *)malloc(n * sizeof(seq_t));
#endif
}

static inline seq_t *seq_realloc(seq_t *p, idx_t n)
{
#ifdef __cplusplus
    return mem_resize(seq_t, p, n);
#else
    return (seq_t *)realloc(p, n * sizeof(seq_t));
#endif
}

seq_t a(idx_t n)
{
    /* Static cache of computed values */
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;   /* number of values currently stored (highest index + 1) */
    static idx_t cache_cap = 0;   /* allocated capacity */

    /* Allocate or grow the cache if necessary */
    if (cache == NULL) {
        cache_cap = n + 1;
        cache = seq_malloc(cache_cap);
        cache_len = 0; /* nothing stored yet */
    } else if (cache_cap < n + 1) {
        cache_cap = n + 1;
        cache = seq_realloc(cache, cache_cap);
    }

    /* Compute missing values up to n */
    for (idx_t i = cache_len; i <= n; ++i) {
        if      (i == 0) cache[0] = (seq_t)0;
        else if (i == 1) cache[1] = (seq_t)0;
        else if (i == 2) cache[2] = (seq_t)1;
        else if (i == 3) cache[3] = (seq_t)3;
        else             cache[i] = 4*cache[i-1] - 5*cache[i-2] + 2*cache[i-3] + cache[i-4];
    }
    cache_len = n + 1; /* update stored length */

    return cache[n];
}
