/*
 * A182249  a(0)=0, a(n) = (a(n-1) * n) OR n.
 * https://oeis.org/A182249
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(0) = 0; a(n) = (a(n-1) * n) OR n
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 12, u64 n <= 20, u128 n <= 33,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* a static cache that grows to hold all values up to n */
static seq_t   *cache = NULL;
static idx_t     cache_len = 0;

/* Allocate or enlarge the cache so that a(n) can be returned directly. */
static void ensure_cache(idx_t n)
{
    if (n < cache_len)
        return;

    size_t new_len = (size_t)n + 1;
    seq_t *tmp = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
    if (!tmp) {
        /* If the allocation fails the sequence cannot be evaluated.
           Terminate as the tests do not recover from this. */
        exit(1);
    }
    cache = tmp;

    if (cache_len == 0) {          /* initialise the very first value */
        cache[0] = 0;
        cache_len = 1;
    }

    /* Fill the cache for all missing indices. */
    for (idx_t i = cache_len; i <= n; ++i) {
        seq_t prod = cache[i - 1] * (seq_t)i;
        cache[i]   = prod | (seq_t)i;
    }

    cache_len = new_len;
}

/* Public function required by the test harness */
seq_t a(idx_t n)
{
    ensure_cache(n);
    return cache[n];
}
