/*
 * A049853  a(n) = a(n-1) + Sum_{k=0..n-3} a(k) for n >= 2, a(0)=1, a(1)=2.
 * https://oeis.org/A049853
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1) + Sum_{k=0..n-3} a(k) a(0) = 1, a(1) = 2, a(2) = 2
 *            This implementation uses the equivalent 3‑term linear recurrence
 *            a(n) = 2*a(n-1) - a(n-2) + a(n-3) for n ≥ 3
 * Range:     u8 n <= 10, u16 n <= 20, u32 n <= 40, u64 n <= 79, u128 n <= 158,
 *            big any n
 * Validated: standard, pass, 50 terms through n = 932 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include "../utils/mem.h"

static seq_t *alloc_seq_array(size_t n)
{
    return mem_new(seq_t, n);
}

static void free_seq_array(seq_t *p)
{
    mem_free(p);
}

static seq_t *cache = NULL;      /* memoised values */
static idx_t cache_len = 0;      /* number of values currently stored */
static idx_t cache_cap = 0;      /* current capacity of cache */

seq_t a(idx_t n)
{
    /* expand cache to hold at least n+1 elements */
    if (n >= cache_cap) {
        seq_t *new_cache = alloc_seq_array(n + 1);
        if (!new_cache)
            abort();                /* allocation error */

        /* copy previously computed values */
        for (idx_t i = 0; i < cache_len; ++i)
            new_cache[i] = cache[i];

        /* free old cache and replace */
        if (cache)
            free_seq_array(cache);
        cache = new_cache;
        cache_cap = n + 1;
    }

    /* compute any missing terms up to n */
    for (idx_t i = cache_len; i <= n; ++i) {
        if (i == 0)
            cache[0] = 1;
        else if (i == 1)
            cache[1] = 2;
        else if (i == 2)
            cache[2] = 2;
        else
            cache[i] = 2 * cache[i-1] - cache[i-2] + cache[i-3];
    }

    cache_len = n + 1;
    return cache[n];
}
