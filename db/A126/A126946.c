/*
 * A126946  The sequence b[n] defined in A126940.
 * https://oeis.org/A126946
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*a(n-1) + a(n-2) + 7*a(n-3) + 4*a(n-4) + 4*a(n-5) +
 *            a(n-6) for n >= 6, with initial values: a(0)=0, a(1)=2, a(2)=4,
 *            a(3)=12, a(4)=40, a(5)=128
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 20, u64 n <= 39, u128 n <= 77,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Cached values of the sequence.
       cache_size  – number of entries currently stored (0 … cache_size-1)
       cache_cap   – allocated capacity of the cache                            */
    static seq_t *cache      = NULL;
    static idx_t cache_size  = 0;
    static size_t cache_cap  = 0;

    /* Ensure the cache is initialized with the first six terms. */
    if (cache_cap < 6) {
        size_t new_cap = 6;
        cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        cache_cap = new_cap;

        cache[0] = 0;
        cache[1] = 2;
        cache[2] = 4;
        cache[3] = 12;
        cache[4] = 40;
        cache[5] = 128;
        cache_size = 6;
    }

    /* If the requested index is already computed, return it immediately. */
    if (n < cache_size)
        return cache[n];

    /* Grow the cache if necessary. */
    if (cache_cap <= (size_t)n) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 16;
        while (new_cap <= (size_t)n)
            new_cap <<= 1;
        cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        cache_cap = new_cap;
    }

    /* Compute the missing terms up to index n using the 6‑term recurrence. */
    for (idx_t i = cache_size; i <= n; ++i) {
        seq_t val = 2 * cache[i - 1];
        val += cache[i - 2];
        val += 7 * cache[i - 3];
        val += 4 * cache[i - 4];
        val += 4 * cache[i - 5];
        val += cache[i - 6];
        cache[i] = val;
    }
    cache_size = n + 1;

    return cache[n];
}
