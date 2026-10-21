/*
 * A077890  Expansion of (1-x)^(-1)/(1+x^2+2*x^3).
 * https://oeis.org/A077890
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   not stated
 * Range:     i8 n <= 15, i16 n <= 31, i32 n <= 46, i64 n <= 46, i128 n <= 46,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n) = a(n-1) - a(n-2) - a(n-3) + 2*a(n-4); 
   Initial values: a(0)=1, a(1)=1, a(2)=0, a(3)=-2 */
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Static cache of already computed terms */
    static seq_t *cache = NULL;
    static size_t sz = 0;          /* current number of cached terms */

    /* Ensure we have at least four terms cached (base case) */
    if (sz < 4) {
        /* Allocate space for four values */
        size_t new_sz = 4;
        seq_t *new_cache = (seq_t *)malloc(new_sz * sizeof(seq_t));
        new_cache[0] = (seq_t)1;
        new_cache[1] = (seq_t)1;
        new_cache[2] = (seq_t)0;
        new_cache[3] = (seq_t)-2;
        if (cache) free(cache);
        cache = new_cache;
        sz = new_sz;
    }

    /* If the requested index is already cached, just return it */
    if ((idx_t)sz > n) return cache[(size_t)n];

    /* Grow cache up to n+1 */
    size_t new_sz = (size_t)n + 1;
    seq_t *new_cache = (seq_t *)realloc(cache, new_sz * sizeof(seq_t));
    if (!new_cache) return 0;   /* allocation failed – should never happen in tests */
    cache = new_cache;

    /* Compute missing terms using the recurrence */
    for (size_t i = sz; i <= n; ++i) {
        seq_t t1 = cache[i - 1];
        seq_t t2 = cache[i - 2];
        seq_t t3 = cache[i - 3];
        seq_t t4 = cache[i - 4];
        cache[i] = t1 - t2 - t3 + ((t4 << 1) + (t4 & 0)); /* t4*2 = t4<<1 for integral types */
    }

    sz = new_sz;
    return cache[(size_t)n];
}
