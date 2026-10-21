/*
 * A078067  Expansion of (1-x)/(1+2*x+2*x^2-2*x^3).
 * https://oeis.org/A078067
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = -2*a(n-1) - 2*a(n-2) + 2*a(n-3), with a(0)=1, a(1)=-3,
 *            a(2)=4
 * Range:     i8 n <= 7, i16 n <= 16, i32 n <= 34, i64 n <= 35, i128 n <= 35,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache to memoize computed terms */
    static seq_t *cache = NULL;
    static idx_t cache_cap = 0;
    static idx_t cache_len = 0;

    /* initialise cache with the first three terms */
    if (cache == NULL) {
        cache_cap = 3;
        cache = (seq_t*)malloc(cache_cap * sizeof(seq_t));
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)(-3);
        cache[2] = (seq_t)4;
        cache_len = 3;
    }

    /* return if already computed */
    if (n < cache_len)
        return cache[n];

    /* enlarge cache if necessary */
    if (n >= cache_cap) {
        idx_t new_cap = cache_cap;
        while (new_cap <= n) new_cap <<= 1;          /* double until sufficient */
        cache = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        cache_cap = new_cap;
    }

    /* compute new terms up to n */
    for (idx_t i = cache_len; i <= n; ++i) {
        seq_t v = (seq_t)(-2) * cache[i-1]
                + (seq_t)(-2) * cache[i-2]
                + (seq_t) 2 * cache[i-3];
        cache[i] = v;
    }
    cache_len = n + 1;
    return cache[n];
}
