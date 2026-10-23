/*
 * A180920  Numbers k such that the sum of the cubes of the k consecutive
 *          integers starting from k is a square.
 * https://oeis.org/A180920
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 63*a(n-1) - 63*a(n-2) + a(n-3) with a(1)=1, a(2)=33,
 *            a(3)=2017. Derived from the Pell equation (3n-1)(5n-3) = square
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 6, u64 n <= 11, u128 n <= 22,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;       /* 1‑based array of already computed terms */
    static idx_t cache_size = 0;      /* highest index in cache (>=0) */
    static size_t cache_cap  = 0;     /* allocated array size */

    if (n == 0) return (seq_t)0;              /* undefined for n=0, guard */

    /* initialise cache on the first call */
    if (cache == NULL) {
        cache_cap = 4;                          /* first 4 elements (idx 0 unused) */
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache) return (seq_t)0;           /* allocation failure */
        cache[0] = 0;                          /* dummy to make array 1‑based */
        cache[1] = (seq_t)1;
        cache[2] = (seq_t)33;
        cache[3] = (seq_t)2017;
        cache_size = 3;
    }

    /* if n is already known, return it */
    if (n <= cache_size) return cache[n];

    /* compute terms up to n */
    for (idx_t i = cache_size + 1; i <= n; ++i) {
        /* make sure there is space */
        if (i >= cache_cap) {
            size_t new_cap = cache_cap * 2;
            seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            if (!tmp) return (seq_t)0;           /* allocation failure */
            cache = tmp;
            cache_cap = new_cap;
        }
        seq_t term = cache[i-1];
        term *= (seq_t)63;
        seq_t t = cache[i-2];
        t *= (seq_t)63;
        term -= t;
        term += cache[i-3];
        cache[i] = term;
    }

    cache_size = n;
    return cache[n];
}
