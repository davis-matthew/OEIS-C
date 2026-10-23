/*
 * A022448  Expansion of 1/((1-x)*(1-5*x)*(1-6*x)*(1-12*x)).
 * https://oeis.org/A022448
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 24*a(n-1) -185*a(n-2) +522*a(n-3) -360*a(n-4) Initial
 *            terms: a(0)=1, a(1)=24, a(2)=391, a(3)=5466
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 8, u64 n <= 17, u128 n <= 35,
 *            big any n
 * Helpers:   seq_alloc, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include "../utils/seq_alloc.h"
#include "../utils/seq_realloc.h"

static seq_t *cache = NULL;
static size_t cache_cap = 0;
static idx_t last_computed = 3;

/* initialise the cache with the first four terms */
static void init_cache(void)
{
    if (cache)
        return;
    cache_cap = 4;
    cache = seq_alloc(cache_cap);           /* allocate array of seq_t */
    cache[0] = (seq_t)1;
    cache[1] = (seq_t)24;
    cache[2] = (seq_t)391;
    cache[3] = (seq_t)5466;
    last_computed = 3;
}

/* grow the cache so that it can hold indices up to n        */
static void ensure_capacity(idx_t n)
{
    if ((size_t)n + 1 <= cache_cap)
        return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 8;
    while (new_cap <= (size_t)n + 1)
        new_cap *= 2;
    cache = seq_realloc(cache, cache_cap, new_cap);
    cache_cap = new_cap;
}

seq_t a(idx_t n)
{
    init_cache();

    if (n <= last_computed)
        return cache[n];

    ensure_capacity(n);

    for (idx_t i = last_computed + 1; i <= n; ++i)
    {
        cache[i] = (seq_t)24 * cache[i - 1]
                 - (seq_t)185 * cache[i - 2]
                 + (seq_t)522 * cache[i - 3]
                 - (seq_t)360 * cache[i - 4];
    }

    last_computed = n;
    return cache[n];
}
