/*
 * A022315  a(n) = a(n-1) + a(n-2) + 1, with a(0) = 0 and a(1) = 10.
 * https://oeis.org/A022315
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 19, u32 n <= 42, u64 n <= 88, u128 n <= 180,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n) = a(n-1) + a(n-2) + 1,  a(0)=0,  a(1)=10 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache for the sequence values */
    static seq_t *cache      = NULL;
    static idx_t cache_size  = 0;
    static idx_t curmax      = 0;       /* the highest index already computed */

    /* trivial case */
    if (n == 0) return (seq_t)0;

    /* ----- allocation / resize ----- */
    if (cache == NULL) {
        /* first allocation */
        idx_t newsize = n + 1;
#ifdef __cplusplus
        cache = new seq_t[newsize];
#else
        cache = (seq_t*)malloc(newsize * sizeof(seq_t));
#endif
        if (cache == NULL) return (seq_t)0;  /* OOM protection */
        cache[0] = (seq_t)0;
        if (newsize > 1) cache[1] = (seq_t)10;
        cache_size = newsize;
        curmax      = (newsize > 1 ? 1 : 0);
    }
    else if (n + 1 > cache_size) {
        /* grow the cache */
        idx_t newsize = n + 1;
#ifdef __cplusplus
        seq_t *tmp = new seq_t[newsize];
        for (idx_t i = 0; i < cache_size; ++i) tmp[i] = cache[i];
        delete [] cache;
        cache = tmp;
#else
        cache = (seq_t*)realloc(cache, newsize * sizeof(seq_t));
#endif
        if (cache == NULL) return (seq_t)0;  /* OOM protection */
        cache_size = newsize;
        /* existing values up to curmax are still valid */
    }

    /* ----- compute missing terms ----- */
    idx_t start = (curmax >= 1 ? curmax + 1 : 2);
    for (idx_t i = start; i <= n; ++i) {
        cache[i] = cache[i-1] + cache[i-2] + (seq_t)1;
    }
    curmax = n;

    return cache[n];
}
