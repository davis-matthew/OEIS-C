/*
 * A134736  a(1) = 5; for n >1, a(n) = a(n-1) + gcd(n, a(n-1)).
 * https://oeis.org/A134736
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1) = 5; for n > 1, a(n) = a(n-1) + gcd(n, a(n-1))
 * Range:     u8 n <= 100, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>          /* size_t */
#include <stdlib.h>          /* malloc, realloc, free */
#include "../utils/gcd.h"    /* gcd(seq_t, seq_t) */

/* Compute OEIS sequence A134736 */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;          /* out of spec; return 0 safely */

    static seq_t *cache  = NULL;   /* memoised values a(1) ... a(cache_len) */
    static size_t cache_cap = 0;   /* allocated capacity of cache */
    static idx_t  cache_len = 0;   /* number of terms stored */

    /* Grow cache if necessary */
    if ((size_t)n > cache_cap) {
        size_t new_cap = n;
        /* optional exponential growth to reduce the number of reallocs */
        if (cache_cap < new_cap)
            new_cap = (cache_cap > 0) ? cache_cap * 2 : 1;
        if (new_cap < (size_t)n)
            new_cap = n;

        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) {
            /* allocation failure: clean up and return 0 */
            free(cache);
            cache = NULL;
            cache_cap = 0;
            cache_len = 0;
            return (seq_t)0;
        }
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Compute any missing terms up to n */
    for (idx_t i = cache_len + 1; i <= n; ++i) {
        seq_t val;
        if (i == 1) {
            val = (seq_t)5;
        } else {
            seq_t prev = cache[(size_t)(i - 2)];   /* a(i-1) */
            seq_t g    = gcd(prev, (seq_t)i);      /* gcd(i, a(i-1)) */
            val = prev + g;
        }
        cache[(size_t)(i - 1)] = val;              /* store a(i) */
    }
    cache_len = n;
    return cache[(size_t)(n - 1)];
}
