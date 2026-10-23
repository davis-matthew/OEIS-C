/*
 * A014743  Expansion of (1+x)/(1-x-x^2-x^4-x^5).
 * https://oeis.org/A014743
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 9, u16 n <= 18, u32 n <= 37, u64 n <= 74, u128 n <= 149,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A014743: expansion of (1+x)/(1-x-x^2-x^4-x^5).
   Recurrence: a(0)=1, a(1)=2, a(2)=3, a(3)=5, a(4)=9, a(5)=17.
   For n>=6: a(n)=a(n-1)+a(n-2)+a(n-4)+a(n-5).  The first two terms
   include an additional +1 due to the numerator 1+x.  */

#include <stdlib.h>

static void ensure_capacity(idx_t n, seq_t **p, size_t *cap)
{
    size_t required = (size_t)n + 1;
    if (required <= *cap) return;
    size_t new_cap = *cap ? *cap : 8;
    while (required > new_cap) new_cap <<= 1;
    *p = (seq_t*)realloc(*p, sizeof(seq_t) * new_cap);
    *cap = new_cap;
}

seq_t a(idx_t n)
{
    /* Static cache for memoization. */
    static seq_t *cache = NULL;
    static size_t cap   = 0;
    static idx_t  last  = 0;
    static int    init  = 0;

    /* Initialise cache on first call. */
    if (!init) {
        cap = 8;
        cache = (seq_t*)malloc(sizeof(seq_t) * cap);
        cache[0] = 1;   /* a(0) */
        last = 0;
        init = 1;
    }

    /* Ensure the cache can hold all indices up to n. */
    ensure_capacity(n, &cache, &cap);

    /* Return cached value if already computed. */
    if (n <= last) return cache[n];

    /* Compute new terms sequentially. */
    for (idx_t i = last + 1; i <= n; ++i) {
        seq_t val = cache[i-1];
        if (i >= 2) val += cache[i-2];
        if (i >= 4) val += cache[i-4];
        if (i >= 5) val += cache[i-5];
        if (i == 1) val += 1;   /* numerator contribution for n=1. */
        cache[i] = val;
    }
    last = n;
    return cache[n];
}
