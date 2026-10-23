/*
 * A157646  Positive numbers y such that y^2 is of the form x^2 + (x+31)^2
 *          with integer x.
 * https://oeis.org/A157646
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 15, u32 n <= 33, u64 n <= 72, u128 n <= 147,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Formula: a(n) = 6*a(n-3) - a(n-6) for n > 6; 
 * initial values: a(1)=25, a(2)=31, a(3)=41, a(4)=109, a(5)=155, a(6)=221
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* 0‑based index is never used; the sequence is 1‑based. */
    if (n == 0) return 0;

    static seq_t *cache     = NULL;  /* array holding computed terms   */
    static idx_t cache_cap  = 0;      /* allocated size of cache (>= n+1) */
    static idx_t max_computed = 0;    /* largest n already computed     */

    /* enlarge cache if needed; ensure capacity > n */
    if (!cache || cache_cap <= n) {
        size_t newcap = cache_cap ? cache_cap : 64;
        while (newcap <= n) newcap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, newcap * sizeof(seq_t));
        if (!tmp) abort();
        cache = tmp;
        cache_cap = newcap;
    }

    /* initialise the first six terms once */
    if (max_computed < 6) {
        cache[1] = (seq_t)25;
        cache[2] = (seq_t)31;
        cache[3] = (seq_t)41;
        cache[4] = (seq_t)109;
        cache[5] = (seq_t)155;
        cache[6] = (seq_t)221;
        max_computed = 6;
    }

    /* compute terms up to n using the recurrence */
    for (idx_t i = max_computed + 1; i <= n; ++i) {
        /* compute 6 * cache[i-3] using only additions (avoids overflow) */
        seq_t tmp = cache[i - 3];
        tmp += cache[i - 3];   /* 2× */
        tmp += tmp;            /* 4× */
        tmp += cache[i - 3];   /* 5× */
        tmp += cache[i - 3];   /* 6× */
        cache[i] = tmp - cache[i - 6];
    }
    max_computed = n;

    return cache[n];
}
