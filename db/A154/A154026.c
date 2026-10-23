/*
 * A154026  a(n+2) = 81*a(n+1) - a(n), a(1)=0, a(2)=9.
 * https://oeis.org/A154026
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 6, u64 n <= 11, u128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(1)=0; a(2)=9; a(n+2)=81*a(n+1)-a(n) */
#include <stddef.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n < 1)
        return (seq_t)0;          /* out‑of‑range guard */

    /* static cache for memoisation */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;  /* number of terms stored (a(1) at cache[0]) */
    static size_t cache_cap = 0;  /* allocated capacity */

    /* initialise cache with first two terms */
    if (cache == NULL) {
        cache_cap = 2;
        cache = (seq_t *)malloc(sizeof(seq_t) * cache_cap);
        if (!cache)
            return (seq_t)0;      /* allocation failure fallback */
        cache[0] = (seq_t)0;     /* a(1) */
        cache[1] = (seq_t)9;     /* a(2) */
        cache_len = 2;
    }

    if ((size_t)n <= cache_len)
        return cache[n - 1];

    /* grow cache if necessary */
    if ((size_t)n > cache_cap) {
        size_t new_cap = cache_cap;
        while (new_cap < (size_t)n)
            new_cap *= 2;
        seq_t *tmp = (seq_t *)realloc(cache, sizeof(seq_t) * new_cap);
        if (!tmp)
            return (seq_t)0;      /* allocation failure fallback */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* compute missing terms up to n */
    for (idx_t i = (idx_t)(cache_len + 1); i <= n; ++i) {
        seq_t prev    = cache[i - 2];   /* a(i-1) */
        seq_t prevprev = cache[i - 3];  /* a(i-2) */
        seq_t next    = prev * (seq_t)81 - prevprev;
        cache[i - 1] = next;            /* store a(i) */
    }
    cache_len = n;
    return cache[n - 1];
}
