/*
 * A160082  Numerator of Hermite(n, 21/26).
 * https://oeis.org/A160082
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, frac
 * Formula:   a(n) = 21*a(n-1) - 338*(n-1)*a(n-2) * a(0) = 1 * a(1) = 21 * *
 *            Computes the nth term of OEIS A160082: Numerator of Hermite(n,
 *            21/26). * Values are stored in a growing cache so that calls in
 *            any order are O(1) * after the initial computation
 * Range:     i8 n <= 2, i16 n <= 3, i32 n <= 7, i64 n <= 11, i128 n <= 22,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* persistent cache of computed terms */
    static seq_t *cache = NULL;      /* array of a(0)..a(last_computed) */
    static size_t cap   = 0;         /* allocated size of cache            */
    static size_t sz    = 0;         /* number of valid entries in cache   */

    /* initialise cache to hold the first two terms */
    if (sz == 0) {
        size_t ncap = 2;
        cache = (seq_t *)malloc(ncap * sizeof(seq_t));
        if (!cache) abort();
        cap = ncap;
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)21;
        sz = 2;
    }

    /* enlarge cache if required for the requested index */
    if ((size_t)n + 1ULL > cap) {
        size_t newcap = cap ? cap * 2 : 2;
        while (newcap <= (size_t)n + 1ULL) newcap <<= 1;
        cache = (seq_t *)realloc(cache, newcap * sizeof(seq_t));
        if (!cache) abort();
        cap = newcap;
    }

    /* compute missing terms up to n */
    for (size_t i = sz; i <= (size_t)n; ++i) {
        if (i < 2) continue;  /* a(0) and a(1) already set */

        /* term = 21*a(i-1) - 338*(i-1)*a(i-2) */
        seq_t term = 21 * cache[i-1];
        term -= (seq_t)338 * ((seq_t)(i-1) * cache[i-2]);
        cache[i] = term;
    }

    sz = (size_t)n + 1ULL;
    return cache[(size_t)n];
}
