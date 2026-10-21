/*
 * A224881  Expansion of 1/(1 - 16*x)^(1/8).
 * https://oeis.org/A224881
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (2^n/n!) * Product_{k=0..n-1} (8*k+1)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 17, u128 n <= 33,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/gcd.h"

static seq_t *cache = NULL;
static idx_t cache_len = 0;
static idx_t max_computed = 0;
static int initialized = 0;

/* Ensure the cache can store at least n+1 terms. */
static void ensure_cache(idx_t n)
{
    if (n < cache_len)
        return;
    idx_t new_len = n + 1;
    if (new_len < 2 * cache_len)
        new_len = 2 * cache_len;
    if (new_len == 0)
        new_len = 1;
    seq_t *tmp = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
    if (!tmp)
        exit(1);
    cache = tmp;
    for (idx_t i = cache_len; i < new_len; ++i)
        cache[i] = 0;
    cache_len = new_len;
}

/* Sequence a(n) */
seq_t a(idx_t n)
{
    if (!initialized) {
        ensure_cache(0);
        cache[0] = (seq_t)1;
        max_computed = 0;
        initialized = 1;
    }
    if (n < max_computed)
        return cache[n];

    ensure_cache(n);

    for (idx_t i = max_computed + 1; i <= n; ++i) {
        seq_t numerator = ((seq_t)16 * (seq_t)(i - 1)) + (seq_t)2;
        seq_t denom     = (seq_t)i;
        seq_t g = gcd(denom, numerator);
        denom        = denom / g;
        numerator    = numerator / g;

        seq_t val = cache[i - 1] / denom;
        val = val * numerator;
        cache[i] = val;
    }
    max_computed = n;
    return cache[n];
}
