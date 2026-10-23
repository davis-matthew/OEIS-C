/*
 * A121517  a(n) = 4*a(n-1) + 4*a(n-2) - a(n-3); a(0)=1, a(1)=1, a(2)=7.
 * https://oeis.org/A121517
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 4*a(n-1) + 4*a(n-2) - a(n-3); a(0)=1, a(1)=1, a(2)=7
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 14, u64 n <= 22, u128 n <= 22,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The function caches computed values and grows the cache as needed. */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache of previously computed terms */
    static seq_t *cache = NULL;
    static idx_t cap  = 0;   /* allocation size (in terms) */
    static idx_t sz   = 0;   /* number of terms stored */

    /* initialisation */
    if (cache == NULL) {
        /* start with space for 3 terms (0,1,2) */
        cap = 3;
        cache = (seq_t *)malloc((size_t)cap * sizeof(seq_t));
        if (!cache) return 0;          /* allocation failure – not expected */
        cache[0] = 1;                  /* a(0) */
        cache[1] = 1;                  /* a(1) */
        cache[2] = 7;                  /* a(2) */
        sz = 3;
    }

    /* if the requested index is already computed, return it */
    if (n < sz) return cache[n];

    /* ensure the cache is large enough */
    if (n >= cap) {
        idx_t newcap = cap * 2;
        while (newcap <= n) newcap *= 2;
        seq_t *tmp = (seq_t *)realloc(cache, (size_t)newcap * sizeof(seq_t));
        if (!tmp) return 0;            /* allocation failure – not expected */
        cache = tmp;
        cap   = newcap;
    }

    /* compute missing terms up to n (inclusive) */
    for (idx_t i = sz; i <= n; ++i) {
        /* a(i) = 4*a(i-1) + 4*a(i-2) - a(i-3) */
        seq_t term = cache[i-1] * 4 + cache[i-2] * 4 - cache[i-3];
        cache[i] = term;
    }
    sz = n + 1;
    return cache[n];
}
