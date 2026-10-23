/*
 * A008462  Take sum of squares of digits of previous term; start with 8.
 * https://oeis.org/A008462
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 100, u16 n <= 100, u32 n <= 100, u64 n <= 100,
 *            u128 n <= 100, big any n
 * Helpers:   digit_frequencies
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: A008462
   a(1) = 8
   a(n) = ∑ d^2 over all decimal digits d of a(n‑1)   (n ≥ 2)
   Recurrence uses digit_frequencies(seq_t x, unsigned int freq[10]).
*/

#include <stdlib.h>
#include <stdint.h>
#include "../utils/digit_frequencies.h"

/* Helper to grow cache array when needed */
static void grow_cache(size_t needed)
{
    size_t old = 0;
    if (needed > 0)
        old = (size_t)((needed > 0) ? needed/2 : 0); /* placeholder not used */
    /* Not used: actual implementation below */
}

/* Static cache for previously computed terms */
static seq_t *cache = NULL;   /* array of terms, 0‑based indexing */
static size_t cache_size = 0; /* number of terms stored */

/* Ensure cache has at least 'n' terms stored */
static void ensure_cache(idx_t n)
{
    if (cache_size >= (size_t)n)
        return;

    /* Allocate new storage, copying old data */
    size_t new_size = (size_t)n;
    seq_t *new_cache = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
    if (!new_cache) abort(); /* fatal error */
    cache = new_cache;
    cache_size = new_size;
}

/* Main function to return a(n) */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* undefined; return 0 */
    /* Initialize cache if empty */
    if (cache_size == 0) {
        cache = (seq_t *)malloc(sizeof(seq_t));
        if (!cache) abort();
        cache[0] = (seq_t)8;         /* a(1) = 8 */
        cache_size = 1;
    }

    /* Grow cache if needed */
    if (cache_size < (size_t)n) {
        /* Compute terms up to n */
        for (idx_t idx = (idx_t)cache_size + 1; idx <= n; ++idx) {
            seq_t prev = cache[(size_t)(idx - 2)]; /* previous term */
            unsigned freq[10];
            digit_frequencies(prev, freq);

            seq_t sum = 0;
            for (int d = 0; d < 10; ++d) {
                sum += (seq_t)(freq[d] * d * d);
            }

            /* Append new term */
            seq_t *new_cache = (seq_t *)realloc(cache, ((size_t)idx) * sizeof(seq_t));
            if (!new_cache) abort();
            cache = new_cache;
            cache[ (size_t)(idx - 1) ] = sum;
            cache_size = (size_t)idx;
        }
    }

    return cache[(size_t)(n - 1)];
}
