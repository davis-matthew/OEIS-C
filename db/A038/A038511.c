/*
 * A038511  Composite numbers with smallest prime factor >= 11.
 * https://oeis.org/A038511
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 8440, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A038511: Composite numbers with smallest prime factor >= 11.
   The function builds the sequence by scanning integers, skipping
   numbers divisible by 2,3,5,or 7 and keeping only composite numbers.
   It keeps a cache so that calls can come in any order. */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/is_prime.h"

/* External helper: bool is_prime(idx_t x);  (provided by utils) */

static seq_t *cache = NULL;           /* cached values */
static size_t cache_size = 0;         /* number of cached terms */
static size_t cache_cap = 0;          /* allocated capacity */
static idx_t  next_candidate = 121;   /* next integer to test */

/* Extend the cache until it contains at least n terms. */
static void ensure_cache_size(idx_t n)
{
    while ((idx_t)cache_size < n)
    {
        /* Find next composite that is not divisible by 2,3,5,7. */
        for (;; ++next_candidate)
        {
            if ((next_candidate & 1ULL) &&
                (next_candidate % 3ULL) &&
                (next_candidate % 5ULL) &&
                (next_candidate % 7ULL) &&
                !is_prime(next_candidate))
                break;
        }

        /* Grow the cache if needed. */
        if (cache_size == cache_cap)
        {
            size_t new_cap = (cache_cap == 0) ? 16 : cache_cap * 2;
            seq_t *new_cache = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
            if (!new_cache) abort();          /* allocation failure */
            cache = new_cache;
            cache_cap = new_cap;
        }

        cache[cache_size++] = (seq_t)next_candidate;
        ++next_candidate;   /* advance to the next potential candidate */
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;  /* offset starts at 1, but guard against 0 */
    ensure_cache_size(n);
    return cache[n-1];
}
