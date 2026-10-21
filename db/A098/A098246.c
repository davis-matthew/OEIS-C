/*
 * A098246  Chebyshev polynomials S(n,227) + S(n-1,227) with Diophantine
 *          property.
 * https://oeis.org/A098246
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 4, u64 n <= 8, u128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A098246:
   a(n) = 227*a(n-1) - a(n-2)   (n > 1)
   a(0) = 1, a(1) = 228
   (equivalent to a(n) = S(n,227)+S(n-1,227) or generating function (1+x)/(1-227x+x^2)) */
#include <stdlib.h>
#include <stddef.h>

/* External types provided by the build */
seq_t a(idx_t n)
{
    /* static cache for memoisation */
    static seq_t *cache = NULL;
    static size_t cache_size = 0;   /* allocated array size */
    static size_t cache_len  = 0;   /* number of elements currently stored (next free index) */

    /* ensure base cases are stored in the cache */
    if (cache_len < 2) {
        size_t needed = 2;
        if (cache_size < needed) {
            seq_t *tmp = (seq_t *)realloc(cache, needed * sizeof(seq_t));
            cache = tmp;
            cache_size = needed;
        }
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)228;
        cache_len = 2;
    }

    /* if the index is already computed, just return it */
    if ((size_t)n < cache_len) {
        return cache[n];
    }

    /* grow the cache if necessary so that index n can be filled */
    size_t new_size = cache_len;
    while (new_size <= (size_t)n) {
        new_size <<= 1;      /* double until we can hold index n */
    }
    if (new_size > cache_size) {
        seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        cache = tmp;
        cache_size = new_size;
    }

    /* compute the missing terms up to n */
    for (idx_t i = (idx_t)cache_len; i <= n; ++i) {
        cache[i] = (seq_t)227 * cache[i-1] - cache[i-2];
    }
    cache_len = (size_t)(n + 1);

    return cache[n];
}
