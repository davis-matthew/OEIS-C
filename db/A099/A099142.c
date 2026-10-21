/*
 * A099142  a(n) = 6^n * T(n, 4/3) where T is the Chebyshev polynomial of the
 *          first kind.
 * https://oeis.org/A099142
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 16·a(n-1) – 36·a(n-2) with a(0) = 1, a(1) = 8 (OEIS
 *            A099142)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 17, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>   /* malloc, realloc */

/* External linkage: the build will supply seq_t and idx_t typedefs. */
seq_t a(idx_t n)
{
    /* A static cache to store already computed terms. */
    static seq_t *cache = NULL;     /* array of terms */
    static size_t   cached = 0;     /* number of terms stored (cache[0..cached-1]) */
    static size_t   cap = 0;        /* allocated capacity */

    /* Initialise the cache with base values. */
    if (cache == NULL) {
        /* start with a modest capacity and grow on demand */
        cap = 16;
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
        cache[0] = (seq_t)1;        /* a(0) */
        cache[1] = (seq_t)8;        /* a(1) */
        cached = 2;                 /* two terms stored */
    }

    /* If the requested index is already cached, simply return it. */
    if ((size_t)n < cached) {
        return cache[n];
    }

    /* Grow the cache if necessary. */
    if ((size_t)n >= cap) {
        size_t newcap = cap;
        while (newcap <= (size_t)n) {
            newcap *= 2;
        }
        seq_t *tmp = (seq_t *)realloc(cache, newcap * sizeof(seq_t));
        cache = tmp;
        cap = newcap;
    }

    /* Compute all missing terms up to n using the recurrence. */
    for (size_t i = cached; i <= (size_t)n; ++i) {
        seq_t prev1 = cache[i - 1];
        seq_t prev2 = cache[i - 2];

        /* 16 * prev1  -  36 * prev2  */
        seq_t t1 = prev1 * (seq_t)16;
        seq_t t2 = prev2 * (seq_t)36;
        cache[i] = t1 - t2;
    }

    cached = (size_t)n + 1;
    return cache[n];
}
