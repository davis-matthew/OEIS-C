/*
 * A052960  Expansion of ( 1-x-x^2 ) / ( 1-2*x-2*x^2+x^3+x^4 ).
 * https://oeis.org/A052960
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 12, u32 n <= 24, u64 n <= 47, u128 n <= 95,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Formula: a(n) = 2*a(n-1) + 2*a(n-2) - a(n-3) - a(n-4) 
 * with initial values a(0)=1, a(1)=1, a(2)=3, a(3)=7. 
 */

#include <stdlib.h>

/* seq_t and idx_t are supplied by the build environment. */

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;   /* memoised values */
    static idx_t  len   = 0;       /* number of cached terms */

    /* Initialise the cache on the first call. */
    if (len == 0) {
        /* Allocate capacity for the first four terms. */
        cache = (seq_t*)malloc(4 * sizeof(seq_t));
        if (!cache) return 0; /* unlikely, but safe fallback */
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)1;
        cache[2] = (seq_t)3;
        cache[3] = (seq_t)7;
        len = 4;
    }

    /* If the requested term is already cached, return it. */
    if (n < len)
        return cache[n];

    /* Extend the cache up to n (inclusive). */
    /* Use realloc; ensure we keep the old values. */
    seq_t *tmp = (seq_t*)realloc(cache, (n + 1) * sizeof(seq_t));
    if (!tmp) return 0;          /* allocation failure */
    cache = tmp;

    /* Compute new terms using the recurrence. */
    for (idx_t i = len; i <= n; ++i) {
        /* All arithmetic is performed in seq_t; 
           the recurrence guarantees non‑negative results. */
        seq_t term = (cache[i-1] * (seq_t)2) +
                     (cache[i-2] * (seq_t)2);
        term -= (cache[i-3] + cache[i-4]);
        cache[i] = term;
    }

    len = n + 1;
    return cache[n];
}
