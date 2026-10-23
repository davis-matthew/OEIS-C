/*
 * A025230  a(n) = a(1)*a(n-1) + a(2)*a(n-2) + ...+ a(n-1)*a(1) for n >= 3,
 *          with initial terms 3,1.
 * https://oeis.org/A025230
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = sum_{i=1}^{n-1} a(i) * a(n-i) for n >= 3, with a(1) = 3,
 *            a(2) = 1. This recurrence is equivalent to the given D‑finite
 *            form but avoids intermediate overflow by using convolution
 *            directly
 * Range:     u8 n <= 5, u16 n <= 7, u32 n <= 13, u64 n <= 24, u128 n <= 46,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* cache[i] stores a(i+1); cache_len = number of terms stored */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;
    static size_t cache_cap = 0;

    /* handle the two base cases */
    if (n == 1) return (seq_t)3;
    if (n == 2) return (seq_t)1;

    /* initialise cache on first call */
    if (cache == NULL) {
        cache_cap = 2;
        cache = (seq_t*)malloc(cache_cap * sizeof(seq_t));
        if (!cache) return (seq_t)0;          /* unlikely in test harness */
        cache[0] = (seq_t)3;                  /* a(1) */
        cache[1] = (seq_t)1;                  /* a(2) */
        cache_len = 2;
    }

    /* if requested value already computed, return it */
    if ((size_t)n <= cache_len)
        return cache[n - 1];

    /* grow cache to accommodate n */
    if (n > cache_cap) {
        size_t new_cap = cache_cap;
        while (new_cap < (size_t)n) new_cap <<= 1;
        seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return (seq_t)0;          /* unlikely */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* compute missing terms using convolution */
    for (size_t m = cache_len; m < (size_t)n; ++m) {
        /* m is the index of the term to compute, 0‑based, so
           a(m+1) = sum_{i=0}^{m-1} a(i+1)*a(m-i) */
        seq_t sum = 0;
        for (size_t i = 0; i < m; ++i) {
            size_t j = m - 1 - i;
            sum += cache[i] * cache[j];
        }
        cache[m] = sum;
    }

    cache_len = (size_t)n;
    return cache[n - 1];
}
