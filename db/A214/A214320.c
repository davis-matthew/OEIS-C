/*
 * A214320  a(0)=a(1)=a(2)=1; thereafter a(n) = gpf(a(n-1)+a(n-3)), where gpf
 *          = "greatest prime factor".
 * https://oeis.org/A214320
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0)=a(1)=a(2)=1; a(n)=gpf(a(n-1)+a(n-3)), n>=3 the function
 *            memoizes computed values for out‑of‑order calls
 * Range:     u8 n <= 120, u16 n <= 120, u32 n <= 120, u64 n <= 120,
 *            u128 n <= 120, big any n
 * Helpers:   gpf
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>
#include <stdint.h>
#include "../utils/gpf.h"

/* static cache for memoization */
static seq_t *cache = NULL;
static size_t cache_cap = 0;   /* allocated capacity */
static idx_t  cache_len = 0;   /* number of values computed */

/* ensure the cache has capacity for at least 'needed' elements */
static void cache_ensure_capacity(size_t needed)
{
    if (needed <= cache_cap)
        return;
    size_t new_cap = cache_cap ? cache_cap << 1 : 4;
    while (new_cap < needed)
        new_cap <<= 1;
    seq_t *new_arr = (seq_t *)malloc(new_cap * sizeof(seq_t));
    if (!new_arr)         /* allocation failure: abort silently */
        return;
    if (cache) {
        for (size_t i = 0; i < cache_len; ++i)
            new_arr[i] = cache[i];
        free(cache);
    }
    cache = new_arr;
    cache_cap = new_cap;
}

/* The main sequence function */
seq_t a(idx_t n)
{
    /* if we have no values yet, initialize the first three */
    if (cache_len < 3) {
        cache_ensure_capacity(3);
        cache[0] = 1;
        cache[1] = 1;
        cache[2] = 1;
        cache_len = 3;
    }

    /* return early if we already know the value */
    if (n < cache_len)
        return cache[(size_t)n];

    /* guarantee capacity for up to index n */
    if ((size_t)(n + 1) > cache_cap)
        cache_ensure_capacity((size_t)(n + 1));

    /* compute missing entries up to n */
    for (idx_t i = cache_len; i <= n; ++i) {
        if (i <= 2) {
            cache[(size_t)i] = 1;
            continue;
        }
        uint64_t sum = (uint64_t)cache[(size_t)(i - 1)] +
                       (uint64_t)cache[(size_t)(i - 3)];
        uint64_t p   = gpf(sum);
        cache[(size_t)i] = (seq_t)p;
    }
    cache_len = (size_t)n + 1;
    return cache[(size_t)n];
}
