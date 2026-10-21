/*
 * A062393  a(n) = n^5 - (n-1)^5 + (n-2)^5 - ... +(-1)^n*0^5.
 * https://oeis.org/A062393
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n^5 - a(n-1), a(0) = 0
 * Range:     u8 n <= 3, u16 n <= 10, u32 n <= 96, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* (Equivalent: a(n) = n^5 - (n-1)^5 + ... + (-1)^n*0^5) */

#include <stdlib.h>

seq_t a(idx_t n) {
    /* Static cache of computed values.  cache[0] = 0 */
    static seq_t *cache = NULL;      /* pointer to array of seq_t values */
    static idx_t cap   = 0;          /* allocated size of the array */
    static idx_t len   = 0;          /* number of valid terms in the cache */

    /* Initialise the cache if not already done */
    if (cap == 0) {
        cap = 32;                     /* initial capacity */
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
        if (cache == NULL) return (seq_t)0;  /* malloc failed: undefined, but guard */
        cache[0] = (seq_t)0;          /* a(0) = 0 */
        len = 1;
    }

    /* Grow the cache if necessary */
    if (n >= cap) {
        idx_t newcap = cap;
        while (newcap <= (idx_t)n) newcap <<= 1;  /* double until big enough */
        seq_t *tmp = (seq_t *)realloc(cache, newcap * sizeof(seq_t));
        if (tmp == NULL) return (seq_t)0;         /* realloc failed */
        cache = tmp;
        cap = newcap;
    }

    /* Compute missing terms up to n using the recurrence a(k) = k^5 - a(k-1) */
    for (idx_t k = len; k <= n; ++k) {
        /* Compute k^5 using a 128‑bit intermediate to avoid overflow */
        __int128 t = (__int128)((seq_t)k);
        t *= t;                 /* k^2 */
        t *= t;                 /* k^4 */
        t *= (__int128)(k);     /* k^5 */
        seq_t val5 = (seq_t)t;
        cache[k] = val5 - cache[k - 1];
    }
    len = n + 1;

    return cache[n];
}
