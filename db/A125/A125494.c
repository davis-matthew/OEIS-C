/*
 * A125494  Composite evil numbers.
 * https://oeis.org/A125494
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 111, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Composite Evil Numbers (A125494)
   a(n) = the n‑th composite integer whose binary representation
   contains an even number of 1’s (an "evil" number). */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/is_prime_small.h"

/* Cached sequence values. */
static seq_t   *cache      = NULL;
static size_t   cache_len  = 0;
static size_t   cache_cap  = 0;
static idx_t    next_candidate = 4;   /* smallest composite to test */

/* Portable popcount for 64‑bit values. */
static inline unsigned int popcount_u64(uint64_t x)
{
#if defined(__GNUC__) || defined(__clang__)
    return __builtin_popcountll(x);
#else
    unsigned int c = 0;
    while (x) {
        c += (unsigned int)(x & 1);
        x >>= 1;
    }
    return c;
#endif
}

/* Grow the cache array when needed. */
static void grow_cache(void)
{
    if (cache_len == cache_cap) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 32;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();      /* fatal: out of memory */
        cache      = tmp;
        cache_cap  = new_cap;
    }
}

/* Return the n‑th term of the sequence (n ≥ 1). */
seq_t a(idx_t n)
{
    if (n == 0) return 0;  /* defined but not used for OEIS offset */

    /* Generate terms until we have at least n of them. */
    while (cache_len < n) {
        uint64_t v = (uint64_t)next_candidate;

        /* Check compositeness: primes are excluded. */
        bool composite = (v >= 4) && !is_prime_small((seq_t)v);

        /* Evil: an even number of 1‑bits. */
        bool evil = ((popcount_u64(v) & 1U) == 0U);

        if (composite && evil) {
            grow_cache();
            cache[cache_len++] = (seq_t)v;
        }
        ++next_candidate;
    }

    return cache[n - 1];
}
