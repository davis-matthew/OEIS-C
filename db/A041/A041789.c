/*
 * A041789  Denominators of continued fraction convergents to sqrt(415).
 * https://oeis.org/A041789
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   a(n) = denominators of continued fraction convergents to
 *            sqrt(415)
 * Range:     u8 n <= 6, u16 n <= 11, u32 n <= 20, u64 n <= 42, u128 n <= 82,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Sequence computation with memoisation */
seq_t a(idx_t n)
{
    /* static cache: cache[i] = a(i) */
    static seq_t *cache   = NULL;   /* pointer to cached values */
    static size_t cache_cap = 0;    /* allocated array size (in elements) */
    static idx_t  cached_up_to = 0; /* highest pre‑computed index */

    /* initialise cache if first call */
    if (cache == NULL) {
        cache_cap = 1;
        cache = (seq_t *)calloc(cache_cap, sizeof(seq_t));
        if (cache == NULL) return (seq_t)0;      /* allocation failure */
        cache[0] = (seq_t)1;                     /* a(0) = 1 */
        cached_up_to = 0;
    }

    /* grow cache if necessary */
    if ((idx_t)(n + 1) > (idx_t)cache_cap) {
        size_t new_cap = cache_cap ? cache_cap : 1;
        while ((size_t)(n + 1) > new_cap)
            new_cap *= 2;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (tmp == NULL) return (seq_t)0;        /* allocation failure */
        cache = tmp;
        /* initialise new slots to zero (not strictly required) */
        for (size_t i = cache_cap; i < new_cap; ++i)
            cache[i] = (seq_t)0;
        cache_cap = new_cap;
    }

    /* if requested value already cached, return it */
    if (n <= cached_up_to)
        return cache[n];

    /* repeated part of the continued fraction for sqrt(415) beyond a0 */
    static const unsigned int repeat[16] = {
        2, 1, 2, 4, 6, 1, 1, 3,
        1, 1, 6, 4, 2, 1, 2, 40
    };

    /* prepare previous two convergents */
    seq_t prev2 = (cached_up_to >= 1) ? cache[cached_up_to - 1] : (seq_t)0;   /* a(n-2) */
    seq_t prev1 = cache[cached_up_to];                                         /* a(n-1) */

    /* compute missing terms up to n */
    for (idx_t i = cached_up_to + 1; i <= n; ++i) {
        seq_t a_i;
        if (i == 0) {
            a_i = (seq_t)20; /* a0 of the CF */
        } else {
            a_i = (seq_t)repeat[(i - 1) % 16];
        }
        seq_t cur = a_i * prev1 + prev2;
        cache[i] = cur;
        prev2 = prev1;
        prev1 = cur;
    }

    cached_up_to = n;
    return cache[n];
}
