/*
 * A189374  Expansion of 1/((1-x)^5*(x^2+x+1)^3).
 * https://oeis.org/A189374
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 14, u16 n <= 42, u32 n <= 42, u64 n <= 42, u128 n <= 42,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A189374 – expansion of 1/((1‑x)^5*(x^2+x+1)^3) */
#include <stdlib.h>

/* recurrence: a(n) = (2*a(n-1) + 2*a(n-2) + (8+n)*a(n-3))/n   (n ≥ 3)
   with a(0)=1, a(1)=2, a(2)=3, a(3)=7 */
seq_t a(idx_t n)
{
    /* static cache for computed terms */
    static seq_t *cache = NULL;   /* pointer to cached values   */
    static size_t cap   = 0;      /* current allocated capacity */
    static size_t len   = 0;      /* number of terms already stored */

    /* first allocation – reserve space for at least 4 terms */
    if (cache == NULL) {
        cap = 4;
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
        if (cache == NULL) return 0;          /* allocation failed */
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)2;
        cache[2] = (seq_t)3;
        cache[3] = (seq_t)7;
        len = 4;
    }

    /* grow the cache if necessary */
    if ((idx_t)cap <= n) {
        size_t newcap = cap * 2;
        if (newcap <= (size_t)n) newcap = (size_t)n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, newcap * sizeof(seq_t));
        if (tmp == NULL) return 0;          /* allocation failed */
        cache = tmp;
        cap = newcap;
    }

    /* compute missing terms up to n */
    for (idx_t i = (idx_t)len; i <= n; ++i) {
        /* recurrence – all arithmetic is done in seq_t */
        seq_t term = ((seq_t)2 * cache[i - 1] +
                      (seq_t)2 * cache[i - 2] +
                      ((seq_t)(8 + i)) * cache[i - 3])/
                     (seq_t)i;
        cache[i] = term;
    }
    /* update the length after computation */
    if ((idx_t)len <= n) len = n + 1;

    return cache[n];
}
