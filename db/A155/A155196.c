/*
 * A155196  a(n)=7*a(n-1)+a(n-2), n>2 ; a(0)=1, a(1)=6, a(2)=42 .
 * https://oeis.org/A155196
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 7*a(n-1)+a(n-2) with a(0)=1, a(1)=6, a(2)=42
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 11, u64 n <= 19, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Cache of computed terms */
static seq_t *cache = NULL;
static size_t cache_len = 0;   /* number of valid entries in cache */
static size_t cache_cap = 0;   /* allocated capacity */

/* Ensure that cache contains values up to index n (inclusive) */
static void ensure_cache(idx_t n)
{
    if (cache_len <= n) {
        /* Grow the cache if necessary */
        size_t new_cap = cache_cap ? cache_cap : 4;
        while (new_cap <= (size_t)n) {
            new_cap <<= 1;
        }

        /* Reallocate (works for both built‑in and big‑int types) */
        seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!new_cache) {
            exit(EXIT_FAILURE);       /* out of memory – abort */
        }

        /* Initialise base terms on first allocation */
        if (cache_len == 0) {
            new_cache[0] = (seq_t)1;
            new_cache[1] = (seq_t)6;
            new_cache[2] = (seq_t)42;
            cache_len = 3;
            /* Zero remaining slots – safe for both integer and big‑int types */
            for (size_t i = 3; i < new_cap; ++i) {
                new_cache[i] = (seq_t)0;
            }
        }

        cache     = new_cache;
        cache_cap = new_cap;
    }

    /* Compute any missing terms up to n */
    if (cache_len <= (size_t)n) {
        for (idx_t i = cache_len; i <= n; ++i) {
            cache[i] = cache[i - 1] * (seq_t)7 + cache[i - 2];
        }
        cache_len = (size_t)n + 1;
    }
}

seq_t a(idx_t n)
{
    ensure_cache(n);
    return cache[(size_t)n];
}
