/*
 * A028204  Expansion of 1/((1-6*x)*(1-7*x)*(1-9*x)*(1-10*x)).
 * https://oeis.org/A028204
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 32*a(n-1) - 379*a(n-2) + 1968*a(n-3) - 3780*a(n-4) a(0) =
 *            1, a(1) = 32, a(2) = 645, a(3) = 10480 (Derived from
 *            1/((1-6x)(1-7x)(1-9x)(1-10x)))
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 8, u64 n <= 17, u128 n <= 36,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

static seq_t *cache = NULL;   /* cache for computed values */
static idx_t  cache_cap = 0;  /* current allocated size of cache */
static idx_t  cache_len = 0;  /* number of values already computed */

seq_t a(idx_t n)
{
    /* Ensure the cache is large enough */
    if ((size_t)n + 1 > cache_cap) {
        size_t new_cap = cache_cap ? cache_cap : 4;
        while (new_cap <= (size_t)n) new_cap <<= 1;
        cache = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        cache_cap = new_cap;
    }

    /* Initialize initial terms if not already present */
    if (cache_len == 0) { cache[0] = 1; cache_len = 1; }
    if (cache_len == 1) { cache[1] = 32; cache_len = 2; }
    if (cache_len == 2) { cache[2] = 645; cache_len = 3; }
    if (cache_len == 3) { cache[3] = 10480; cache_len = 4; }

    /* Compute missing terms up to index n */
    while (cache_len <= n) {
        seq_t a1 = cache[cache_len - 1];
        seq_t a2 = cache[cache_len - 2];
        seq_t a3 = cache[cache_len - 3];
        seq_t a4 = cache[cache_len - 4];
        cache[cache_len] = 32 * a1 - 379 * a2 + 1968 * a3 - 3780 * a4;
        ++cache_len;
    }

    return cache[n];
}
