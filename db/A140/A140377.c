/*
 * A140377  Composites of the form 26k + 3.
 * https://oeis.org/A140377
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, less
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 47, u32 n <= 47, u64 n <= 47, u128 n <= 47,
 *            big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A140377: nth composite congruent to 3 modulo 26.
   Returns the n‑th number of the form 26*k + 3 that is not prime. */
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime_big.h"

static seq_t *cache = NULL;
static size_t cache_cap = 0;
static idx_t cached_len = 0;   /* number of terms stored in cache      */
static idx_t next_k    = 0;    /* next k to test (k ≥ 0)                */

/* ensure the cache has room for at least one more term */
static void grow_cache(void)
{
    if (cache_cap == 0) {
        cache_cap = 8;
        cache = (seq_t*)malloc(cache_cap * sizeof(seq_t));
    } else {
        cache_cap <<= 1; /* double the capacity */
        cache = (seq_t*)realloc(cache, cache_cap * sizeof(seq_t));
    }
}

seq_t a(idx_t n)
{
    /* Extend the cache until it contains at least n terms */
    while (cached_len < n) {
        seq_t candidate;
        /* search for the next composite of the form 26*k+3 */
        do {
            candidate = 26 * (seq_t)next_k + 3;
            next_k++;
        } while (is_prime_big(candidate));

        /* store the new composite */
        if (cached_len >= cache_cap)
            grow_cache();
        cache[cached_len] = candidate;
        cached_len++;
    }

    return cache[n-1];
}
