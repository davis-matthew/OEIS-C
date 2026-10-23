/*
 * A180435  a(n) = a(n-1)*2^n+n, a(0)=1.
 * https://oeis.org/A180435
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 1, a(n) = a(n-1) * 2^n + n (n ≥ 1)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 7, u64 n <= 10, u128 n <= 14,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/big_pow.h"          /* provides seq_t big_pow(seq_t base, seq_t exp) */

static seq_t *cache = NULL;      /* cache of computed values */
static idx_t cache_size = 0;     /* number of valid entries in cache */
static idx_t cache_capacity = 0; /* allocated size of cache */

/* Ensure the cache has space for at least (needed) entries (≤ needed). */
static void
ensure_capacity(idx_t needed)
{
    if (cache_capacity >= needed)
        return;

    idx_t new_cap = cache_capacity ? cache_capacity : 1;
    while (new_cap < needed)
        new_cap *= 2;

    seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp) { /* allocation failed; terminate program */
        abort();
    }
    cache = tmp;
    cache_capacity = new_cap;
}

seq_t a(idx_t n)
{
    if (n >= cache_size) {
        /* Extend cache to hold values up to n */
        ensure_capacity(n + 1);
        for (idx_t i = cache_size; i <= n; ++i) {
            if (i == 0) {
                cache[0] = (seq_t)1;
            } else {
                /* Compute 2^i in seq_t */
                seq_t pow = big_pow((seq_t)2, (seq_t)i);
                cache[i] = cache[i - 1] * pow + (seq_t)i;
            }
        }
        cache_size = n + 1;
    }
    return cache[n];
}
