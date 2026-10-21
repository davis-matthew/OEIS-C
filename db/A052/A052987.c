/*
 * A052987  Expansion of (1-2x^2)/(1-2x-2x^2+2x^3).
 * https://oeis.org/A052987
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 12, u32 n <= 24, u64 n <= 28, u128 n <= 28,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0)=1, a(1)=2, a(2)=4,
   a(n)=2*a(n-1)+2*a(n-2)-2*a(n-3)   (n>=3) */
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Persistent cache for memoization */
    static seq_t *cache = NULL;
    static size_t   cap   = 0;   /* allocated size of cache */
    static idx_t   max_cached = (idx_t)-1;  /* highest index computed */

    /* Ensure base terms are available */
    if (cache == NULL) {
        cap = 3;
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
        if (cache == NULL) abort();          /* allocation failure */
        cache[0] = 1;
        cache[1] = 2;
        cache[2] = 4;
        max_cached = 2;
    }

    /* Expand cache if needed */
    size_t required = (size_t)n + 1;
    if (required > cap) {
        size_t new_cap = cap * 2;
        if (new_cap < required) new_cap = required;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (tmp == NULL) abort();
        cache = tmp;
        cap = new_cap;
    }

    /* Compute missing terms up to n */
    if (n > max_cached) {
        for (idx_t i = max_cached + 1; i <= n; ++i) {
            cache[i] = (cache[i-1] * ((seq_t)2))          /* 2*a(i-1) */
                     + (cache[i-2] * ((seq_t)2))          /* 2*a(i-2) */
                     - (cache[i-3] * ((seq_t)2));         /* 2*a(i-3) */
        }
        max_cached = n;
    }

    return cache[n];
}
