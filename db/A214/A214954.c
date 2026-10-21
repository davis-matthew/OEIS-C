/*
 * A214954  a(n) = 3*a(n-1) + 6*a(n-2) + a(n-3), with a(0) = 0, a(1) = 2, and
 *          a(2) = 7.
 * https://oeis.org/A214954
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3*a(n-1) + 6*a(n-2) + a(n-3), a(0)=0, a(1)=2, a(2)=7
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 15, u64 n <= 24, u128 n <= 24,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;  /* allocated cache of sequence values */
    static size_t  cap   = 0;    /* current capacity of cache */
    static idx_t   last  = 0;    /* largest n computed so far */

    /* initialise cache */
    if (cache == NULL) {
        cap = 3;                 /* space for the three known terms */
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
        if (cache == NULL) abort();        /* allocation failure */
        cache[0] = (seq_t)0;
        cache[1] = (seq_t)2;
        cache[2] = (seq_t)7;
        last = 2;
    }

    /* value already computed? */
    if (n <= last) {
        return cache[n];
    }

    /* enlarge cache if necessary */
    size_t required = (size_t)n + 1;
    if (required > cap) {
        size_t new_cap = cap * 2;
        if (required > new_cap) new_cap = required;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (tmp == NULL) abort();            /* allocation failure */
        cache = tmp;
        cap   = new_cap;
    }

    /* compute missing terms up to n */
    for (idx_t i = last + 1; i <= n; ++i) {
        cache[i] = (seq_t)(3 * cache[i - 1] + 6 * cache[i - 2] + cache[i - 3]);
    }

    last = n;
    return cache[n];
}
