/*
 * A106664  Expansion of g.f.: (1-3*x+x^2)/((1-x)*(1+x)*(1-2*x+2*x^2)).
 * https://oeis.org/A106664
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, sign
 * Formula:   a(n) = 2*a(n-1) - a(n-2) - 2*a(n-3) + 2*a(n-4) with a(0) = -1,
 *            a(1) = 1, a(2) = 2, a(3) = 5
 * Range:     i8 n <= 14, i16 n <= 30, i32 n <= 62, i64 n <= 126,
 *            i128 n <= 254, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache of already computed terms   */
    static seq_t *cache = NULL;
    static idx_t cap    = 0;              // current number of stored terms

    /* initialise with the first four values when first called */
    if (cache == NULL) {
        cache = (seq_t *)malloc(4 * sizeof(seq_t));
        if (!cache) abort();              /* allocation failure */
        cache[0] = (seq_t)-1;
        cache[1] = (seq_t)1;
        cache[2] = (seq_t)2;
        cache[3] = (seq_t)5;
        cap     = 4;
    }

    /* trivial cases */
    if (n < 4)
        return cache[n];

    /* extend cache if necessary */
    if (n >= cap) {
        seq_t *tmp = (seq_t *)realloc(cache, (n + 1) * sizeof(seq_t));
        if (!tmp) abort();
        cache = tmp;
        /* compute missing terms only once */
        for (idx_t i = cap; i <= n; ++i) {
            /* a(i) = 2*a(i-1) - a(i-2) - 2*a(i-3) + 2*a(i-4) */
            cache[i] = 2 * cache[i-1] - cache[i-2] - 2 * cache[i-3] + 2 * cache[i-4];
        }
        cap = n + 1;
    }

    return cache[n];
}
