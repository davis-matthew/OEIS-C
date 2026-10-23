/*
 * A171877  a(n) = a(n-1) + a(n-2)*a(n-3) + a(n-4)^a(n-5), with initial terms
 *          0,0,1,1,1.
 * https://oeis.org/A171877
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1) + a(n-2)*a(n-3) + a(n-4)^a(n-5), a(0)=0, a(1)=0,
 *            a(2)=1, a(3)=1, a(4)=1
 * Range:     u8 n <= 9, u16 n <= 11, u32 n <= 11, u64 n <= 12, u128 n <= 12,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include "../utils/big_pow.h"

/* Persistent cache for computed terms */
static seq_t *cache = NULL;
static size_t cache_cap = 0;   /* allocated size of cache (in entries) */
static idx_t  max_computed = 4;/* highest index whose value is known */

/* Ensure cache is large enough for index n */
static void ensure_cache(idx_t n)
{
    /* already large enough? */
    if (cache && (size_t)n + 1 <= cache_cap)
        return;

    size_t need   = (size_t)n + 1;
    size_t new_cap = cache_cap ? cache_cap : 1;
    while (new_cap < need) new_cap <<= 1;

    seq_t *old_cache = cache;
    seq_t *new_cache = (seq_t*)realloc(old_cache, new_cap * sizeof(seq_t));
    if (!new_cache) abort();            /* out of memory */
    cache = new_cache;
    cache_cap = new_cap;

    /* initialise first five terms if this is a fresh allocation */
    if (!old_cache) {
        static const seq_t init[5] = {0, 0, 1, 1, 1};
        for (size_t i = 0; i < 5 && i < cache_cap; ++i)
            cache[i] = init[i];
    }
}

/* Compute a(n) */
seq_t a(idx_t n)
{
    /* return initial values directly */
    if (n <= 4) {
        static const seq_t init[5] = {0, 0, 1, 1, 1};
        return init[(size_t)n];
    }

    ensure_cache(n);

    /* compute missing values up to n, if any */
    if (max_computed < n) {
        for (idx_t i = max_computed + 1; i <= n; ++i) {
            seq_t term1 = cache[i - 1];
            seq_t term2 = cache[i - 2] * cache[i - 3];
            seq_t term3 = big_pow(cache[i - 4], cache[i - 5]); /* 0^0 handled */
            cache[i] = term1 + term2 + term3;
        }
        max_computed = n;
    }

    return cache[n];
}
