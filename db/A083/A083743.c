/*
 * A083743  a(1) = 1; if a(n-1) + n is prime then a(n) = a(n-1) + n, else a(n)
 *          = a(n-1).
 * https://oeis.org/A083743
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 49, u16 n <= 1069, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   isPrimeTrial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A083743
   a(1) = 1;
   for n > 1: a(n) = a(n-1)+n if a(n-1)+n is prime, otherwise a(n-1). */
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/isPrimeTrial.h"

static seq_t *cache = NULL;   /* 1‑based array of computed terms */
static idx_t cache_len = 0;   /* number of terms stored (>=0) */
static idx_t cache_cap = 0;   /* allocated capacity */

/* Ensure the cache can hold indices up to 'needed'. */
static void grow_cache(idx_t needed)
{
    if (cache_cap >= needed)
        return;
    idx_t new_cap = cache_cap ? cache_cap * 2 : 16;
    while (new_cap < needed)
        new_cap *= 2;
    seq_t *tmp = (seq_t *)realloc(cache, (new_cap + 1) * sizeof(seq_t));
    if (!tmp)
        abort();
    cache = tmp;
    cache_cap = new_cap;
}

/* Initialise the cache with a(1)=1. */
static void init_cache(void)
{
    if (cache_len == 0) {
        grow_cache(16);
        cache_len = 1;
        cache[1] = (seq_t)1;   /* a(1) */
        cache[0] = (seq_t)0;   /* unused placeholder */
    }
}

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;   /* behaviour below offset is undefined */

    init_cache();

    if (cache_len < n) {
        grow_cache(n);
        for (idx_t i = cache_len + 1; i <= n; ++i) {
            seq_t cand = cache[i - 1] + (seq_t)i;
            if (isPrimeTrial(cand))
                cache[i] = cand;
            else
                cache[i] = cache[i - 1];
        }
        cache_len = n;
    }
    return cache[n];
}
