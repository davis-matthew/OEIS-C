/*
 * A015530  Expansion of x/(1 - 4*x - 3*x^2).
 * https://oeis.org/A015530
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 4*a(n-1) + 3*a(n-2), a(0) = 0, a(1) = 1
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 15, u64 n <= 29, u128 n <= 58,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache   = NULL;
    static idx_t  cap     = 0;
    static idx_t  len     = 0;

    /* first call: allocate a minimal cache and initialise the first two terms */
    if (cache == NULL) {
        cap = 2;
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
        cache[0] = (seq_t)0;
        cache[1] = (seq_t)1;
        len = 2;
    }

    /* grow cache if the requested index is beyond current capacity */
    if (n >= cap) {
        idx_t newcap = cap;
        while (newcap <= n) newcap <<= 1;          /* double until big enough */
        seq_t *tmp = (seq_t *)realloc(cache, newcap * sizeof(seq_t));
        if (tmp == NULL) abort();                  /* in unlikely error case */
        cache = tmp;
        cap = newcap;
    }

    /* compute all missing terms up to n */
    if (n >= len) {
        for (idx_t i = len; i <= n; ++i) {
            cache[i] = (seq_t)4 * cache[i-1] + (seq_t)3 * cache[i-2];
        }
        len = n + 1;
    }

    return cache[n];
}
