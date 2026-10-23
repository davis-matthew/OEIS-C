/*
 * A047898  a(1) = 6; for n > 0, a(n+1) = a(n) * (sum of digits of a(n)).
 * https://oeis.org/A047898
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 8, u64 n <= 13, u128 n <= 23,
 *            big any n
 * Helpers:   digit_sum
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A047898: a(1) = 6; a(n) = a(n-1) * digit_sum(a(n-1)) for n > 1 */

#include <stdlib.h>
#include <stddef.h>
#include "../utils/digit_sum.h"

/* Memoised cache: cache[0] holds a(1), cache[1] holds a(2), … */
static seq_t *cache = NULL;
static size_t cache_size   = 0;   /* number of elements stored in cache */
static size_t cache_cap    = 0;   /* current capacity of the cache array */

/* Ensure the cache can hold at least 'need' elements. */
static void ensure_capacity(size_t need)
{
    if (need <= cache_cap)
        return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 4;
    if (new_cap < need)
        new_cap = need;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp) {
        abort(); /* allocation failed */
    }
    cache = tmp;
    cache_cap = new_cap;
}

/* Return the n‑th term of A047898 (n ≥ 1). */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                 /* undefined for n == 0 */

    /* Zero‑based index into the cache.  n is unsigned, so n-1 is safe for n>=1. */
    size_t idx = (size_t)n - 1;

    /* Initialise cache if this is the first call. */
    if (!cache) {
        ensure_capacity(4);
        cache[0] = (seq_t)6;   /* a(1) */
        cache_size = 1;
    }

    /* Ensure we have enough room for the requested term. */
    ensure_capacity(idx + 1);

    /* If the term is already computed, just return it. */
    if (idx < cache_size)
        return cache[idx];

    /* Compute all missing terms up to idx. */
    for (size_t i = cache_size; i <= idx; ++i) {
        seq_t prev = cache[i - 1];
        seq_t s    = digit_sum(prev);   /* sum of the decimal digits of prev */
        cache[i]   = prev * s;
    }

    cache_size = idx + 1;
    return cache[idx];
}
