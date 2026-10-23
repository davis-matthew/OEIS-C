/*
 * A185647  Expansion of (1+2x)*(1+2*x^2)/((1-x)*(1+x)*(1-2*x^2)).
 * https://oeis.org/A185647
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0)=1; for n>=1: a(n)=2*a(n-1) if n is odd, otherwise
 *            a(n)=a(n-1)+3
 * Range:     u8 n <= 12, u16 n <= 28, u32 n <= 60, u64 n <= 124,
 *            u128 n <= 252, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The function memoizes results for all n seen so far. */
#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cap    = 0;
    static idx_t  max_computed = 0;

    /* initialise cache on first call */
    if (cache == NULL) {
        cap    = 1;
        cache  = (seq_t *)malloc(cap * sizeof(seq_t));
        if (cache == NULL) return (seq_t)0;          /* unlikely – abort on OOM */
        cache[0] = (seq_t)1;
        max_computed = 0;
    }

    if ((size_t)n >= cap) {
        size_t new_cap = (size_t)n + 1;
        if (new_cap < cap * 2) new_cap = cap * 2;     /* exponential grow */
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (tmp == NULL) return (seq_t)0;             /* abort on OOM */
        cache = tmp;
        cap   = new_cap;
    }

    if (n > max_computed) {
        for (idx_t i = max_computed + 1; i <= n; ++i) {
            if ((i & (idx_t)1) == 0)            /* even */
                cache[i] = cache[i - 1] + (seq_t)3;
            else                                 /* odd */
                cache[i] = cache[i - 1] * (seq_t)2;
        }
        max_computed = n;
    }

    return cache[n];
}
