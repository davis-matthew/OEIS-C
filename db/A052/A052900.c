/*
 * A052900  Expansion of (1-x)/(1-x-3x^3).
 * https://oeis.org/A052900
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 11, u16 n <= 20, u32 n <= 34, u64 n <= 34, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence:  a(0)=1, a(1)=0, a(2)=0;
   a(n) = 3*a(n-3) + a(n-1)   for n >= 3
   This generates the OEIS sequence A052900. */
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache for memoisation */
    static seq_t *cache       = NULL;   /* array of computed terms */
    static idx_t cache_len    = 0;      /* number of terms currently set in cache */
    static idx_t cache_cap    = 0;      /* allocated capacity of cache */

    /* first call: allocate space for the 3 base terms */
    if (cache == NULL) {
        cache_cap = 3;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache) abort();                 /* out of memory */
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)0;
        cache[2] = (seq_t)0;
        cache_len = 3;                       /* terms 0,1,2 are known */
    }

    /* if requested index already computed, just return it */
    if (n < cache_len)
        return cache[n];

    /* grow the allocated array if necessary */
    if (n >= cache_cap) {
        idx_t new_cap = cache_cap > 0 ? cache_cap : 3;
        while (new_cap <= n)
            new_cap *= 2;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();
        cache   = tmp;
        cache_cap = new_cap;
    }

    /* compute missing terms sequentially up to n */
    for (idx_t i = cache_len; i <= n; ++i) {
        unsigned __int128 t = (unsigned __int128)cache[i-3] * 3
                           + (unsigned __int128)cache[i-1];
        cache[i] = (seq_t)t;               /* safe truncation for big‑int builds */
    }

    cache_len = n + 1;
    return cache[n];
}
