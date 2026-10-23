/*
 * A163873  a(n) = n - a(a(n-2)) with a(0) = a(1) = 0.
 * https://oeis.org/A163873
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n - a(a(n-2)); a(0)=a(1)=0
 * Range:     u8 n <= 413, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

static seq_t *cache = NULL;
static idx_t cache_cap = 0;
static idx_t last_computed = 1;

static void init_cache(void)
{
    if (!cache)
    {
        cache_cap = 2;
        cache = (seq_t*)malloc((cache_cap + 1) * sizeof(seq_t));
        cache[0] = 0;
        cache[1] = 0;
        last_computed = 1;
    }
}

static void ensure_cache(idx_t n)
{
    init_cache();
    if (cache_cap <= n)
    {
        idx_t new_cap = cache_cap ? cache_cap : 1;
        while (new_cap <= n)
            new_cap <<= 1;
        cache = (seq_t*)realloc(cache, (size_t)(new_cap + 1) * sizeof(seq_t));
        cache_cap = new_cap;
    }
}

seq_t a(idx_t n)
{
    if (n == 0 || n == 1)
        return (seq_t)0;

    ensure_cache(n);

    for (idx_t i = last_computed + 1; i <= n; ++i)
    {
        seq_t inner_index = cache[i - 2];
        idx_t idx = (idx_t)inner_index;
        seq_t inner_val = cache[idx];
        cache[i] = (seq_t)i - inner_val;
    }

    last_computed = n;
    return cache[n];
}
