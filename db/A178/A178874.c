/*
 * A178874  Partial sums of round(5^n/8).
 * https://oeis.org/A178874
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, less
 * Formula:   a(n) = 6*a(n-1) - 4*a(n-2) - 6*a(n-3) + 5*a(n-4), n > 3 a(0) =
 *            0, a(1) = 1, a(2) = 4, a(3) = 20
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 14, u64 n <= 28, u128 n <= 56,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

static seq_t *cache = NULL;   /* array of computed terms */
static size_t  cache_cap = 0; /* current capacity of the array   */
static idx_t   cache_len = 0; /* number of terms actually stored */

/* Compute the nth term of sequence A178874. */
seq_t a(idx_t n)
{
    /* Ensure base terms are present. */
    if (cache_len == 0) {
        /* Allocate at least 4 slots for the base values. */
        cache_cap = 4;
        cache = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
        if (!cache) abort();

        cache[0] = (seq_t)0;   /* a(0) */
        cache[1] = (seq_t)1;   /* a(1) */
        cache[2] = (seq_t)4;   /* a(2) */
        cache[3] = (seq_t)20;  /* a(3) */
        cache_len = 4;
    }

    /* Expand the cache if the requested index is beyond its capacity. */
    if ((size_t)n + 1 > cache_cap) {
        size_t new_cap = (size_t)n + 1;
        seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!new_cache) abort();
        cache = new_cache;
        cache_cap = new_cap;
    }

    /* If the value has already been computed, return it. */
    if (n < cache_len) {
        return cache[n];
    }

    /* Compute missing terms up to the requested index using the recurrence. */
    for (idx_t i = cache_len; i <= n; ++i) {
        seq_t val = (seq_t)6 * cache[i - 1];
        val -= (seq_t)4 * cache[i - 2];
        val -= (seq_t)6 * cache[i - 3];
        val += (seq_t)5 * cache[i - 4];
        cache[i] = val;
    }
    cache_len = n + 1; /* update the stored length */

    return cache[n];
}
