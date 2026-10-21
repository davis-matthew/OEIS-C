/*
 * A090239  a(n) = A053838(n) + 2 modulo 3.
 * https://oeis.org/A090239
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = mod(n + a([n/3]), 3), a(0)=2
 * Range:     u8 n <= 272, u16 n <= 8365, u32 n <= 8365, u64 n <= 8365,
 *            u128 n <= 8365, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the OEIS sequence A090239: a(0)=2, a(n)= (n + a(n/3)) mod 3 for n>0. */
#include <stdlib.h>

static seq_t *cache = NULL;   /* dynamic array of cached values */
static size_t cache_capacity = 0;
static size_t cache_size = 0;

/* Ensures that cache contains values up to n (inclusive). */
static void ensure_cache(idx_t n)
{
    /* Grow the array if needed */
    if (n >= cache_size) {
        size_t needed = (n + 1);
        size_t new_cap = cache_capacity ? cache_capacity : 1;
        while (new_cap < needed) new_cap <<= 1;     /* double until large enough */

        if (new_cap != cache_capacity) {
            cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            if (!cache) abort();  /* allocation failed */
            cache_capacity = new_cap;
        }

        /* Compute missing terms */
        for (size_t i = cache_size; i <= (size_t)n; ++i) {
            if (i == 0) {
                cache[0] = 2;                 /* a(0) = 2 */
                continue;
            }
            cache[i] = ( ((seq_t)i + cache[i/3]) % 3 );
        }
        cache_size = (size_t)n + 1;
    }
}

seq_t a(idx_t n)
{
    ensure_cache(n);
    return cache[(size_t)n];
}
