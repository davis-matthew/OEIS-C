/*
 * A053757  a(n) = a(a(n-4))+a(n-a(n-4)) if n>2 otherwise a(n)=1.
 * https://oeis.org/A053757
 *
 * Offset:    -1    (a(-1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = a(a(n-4)) + a(n - a(n-4)), a(n) = 1 for n <= 2
 * Range:     u8 n <= 325, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(-1) is the first term, terms are stored in array as cache[n+1] */

#include <stddef.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* cache[n+1] = a(n), where n >= -1.  cache[0] holds a(-1). */
    static seq_t *cache = NULL;
    static size_t cap = 0;
    static idx_t maxn = 2;   /* largest n computed so far (>= -1) */

    /* base values for n = -1, 0, 1, 2 */
    if (n <= 2)
        return (seq_t)1;

    /* initialize cache for indices -1..2 if necessary */
    if (cache == NULL) {
        size_t init = 4;                  /* indices 0..3 inclusive */
        cache = (seq_t *)malloc(init * sizeof(seq_t));
        if (!cache)
            return (seq_t)0;              /* allocation failed */
        for (size_t i = 0; i < init; ++i)
            cache[i] = (seq_t)1;          /* a(-1), a(0), a(1), a(2) */
        cap = init;
        maxn = 2;
    }

    /* extend the cache to hold indices up to n+1 (inclusive) */
    size_t needed = (size_t)(n + 2);      /* indices 0 .. n+1 */
    if (needed > cap) {
        seq_t *tmp = (seq_t *)realloc(cache, needed * sizeof(seq_t));
        if (!tmp)
            return (seq_t)0;              /* allocation failed */
        cache = tmp;
        cap = needed;
    }

    /* compute all new values from maxn+1 up to n */
    for (idx_t i = maxn + 1; i <= n; ++i) {
        size_t idx   = (size_t)(i + 1);           /* array index for a(i) */

        idx_t n4     = i - 4;                     /* i-4 may be -1 */
        size_t idx_n4 = (size_t)(n4 + 1);         /* index for a(i-4) */
        seq_t a_n4    = cache[idx_n4];            /* a(i-4) */

        /* a(a(i-4)) : index = a_n4 + 1 */
        size_t idx_a_n4 = (size_t)(a_n4 + 1);     /* array index for a(a(i-4)) */

        idx_t t       = i - (idx_t)a_n4;          /* i - a(i-4) */
        size_t idx_tgt = (size_t)(t + 1);        /* index for a(t) */

        seq_t val = cache[idx_a_n4] + cache[idx_tgt];
        cache[idx] = val;
    }

    maxn = n;
    return cache[(size_t)(n + 1)];
}
