/*
 * A138034  Expansion of (1+3*x^2)/(1-x+x^2).
 * https://oeis.org/A138034
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = a(n-1) - a(n-2), a(0)=1, a(1)=1, a(2)=3
 * Range:     i8 n <= 104, i16 n <= 104, i32 n <= 104, i64 n <= 104,
 *            i128 n <= 104, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache   = NULL;
    static size_t cache_cap = 0;
    static size_t cache_len = 0;

    /* Return cached value if already computed. */
    if (n < cache_len)
        return cache[n];

    /* Grow cache to hold indices up to n. */
    if (cache_cap <= (size_t)n) {
        size_t new_cap = cache_cap ? cache_cap << 1 : 4;
        while (new_cap <= (size_t)n)
            new_cap <<= 1;

        /* realloc returns void*, cast explicitly for C++ compatibility. */
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Compute missing terms up to n using the recurrence. */
    for (idx_t i = cache_len; i <= n; ++i) {
        if (i == 0) {
            cache[0] = (seq_t)1;
        } else if (i == 1) {
            cache[1] = (seq_t)1;
        } else if (i == 2) {
            cache[2] = (seq_t)3;
        } else {
            cache[i] = cache[i - 1] - cache[i - 2];
        }
    }

    cache_len = n + 1;
    return cache[n];
}
