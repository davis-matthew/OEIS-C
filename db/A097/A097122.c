/*
 * A097122  Expansion of (1-x)^2/((1-x)^3 - 3*x^3).
 * https://oeis.org/A097122
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 3*a(n-1) - 3*a(n-2) + 4*a(n-3); Initial values: a(0)=1,
 *            a(1)=1, a(2)=1
 * Range:     u8 n <= 7, u16 n <= 13, u32 n <= 26, u64 n <= 50, u128 n <= 100,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache for memoized values */
    static seq_t *cache  = NULL;
    static size_t   cap    = 0;
    static size_t   computed = 0;  /* number of terms already computed */

    /* ensure enough capacity for indices up to n */
    if ((size_t)(n + 1) > cap) {
        size_t new_cap = cap ? cap * 2 : 4;
        while (new_cap < (size_t)(n + 1))
            new_cap *= 2;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp)
            return (seq_t)0;              /* allocation failure */
        cache = tmp;
        cap = new_cap;
        /* initialise base cases on first allocation or when cache grows to at least 3 */
        if (computed == 0 && cap > 2) {
            cache[0] = (seq_t)1;
            cache[1] = (seq_t)1;
            cache[2] = (seq_t)1;
        }
    }

    /* compute missing terms up to n */
    for (idx_t i = computed; i <= n; ++i) {
        if (i < 3) {
            /* base values already set */
            continue;
        }
        cache[(size_t)i] =
            (seq_t)3 * cache[(size_t)(i - 1)] -
            (seq_t)3 * cache[(size_t)(i - 2)] +
            (seq_t)4 * cache[(size_t)(i - 3)];
    }
    if ((size_t)(n + 1) > computed)
        computed = (size_t)(n + 1);

    return cache[(size_t)n];
}
