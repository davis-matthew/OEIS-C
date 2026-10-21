/*
 * A061242  Primes of the form 9*k - 1.
 * https://oeis.org/A061242
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 9, u16 n <= 805, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Iterable generating primes of the form 9*k-1 (congruent to 8 mod 9). */
/* The function memoises results so calls may come in any order. */
/* The code uses the provided is_prime_big() routine for primality testing. */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime_big.h"

static seq_t *cache = NULL;          /* cached primes */
static size_t cache_cap = 0, cache_len = 0;  /* capacity and current length */
static idx_t next_k = 2;             /* next k to test (9*k-1) */

/* Extend cache until it holds at least n terms. */
static void ensure_cache(idx_t n)
{
    while (cache_len < (size_t)n)
    {
        /* Grow the cache if needed. */
        if (cache_len == cache_cap)
        {
            size_t new_cap = cache_cap ? cache_cap * 2 : 16;
            seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            if (!new_cache)
                abort();              /* allocation failed */
            cache = new_cache;
            cache_cap = new_cap;
        }

        /* Candidate of the form 9*k-1. */
        idx_t candidate_idx = (idx_t)9 * next_k - (idx_t)1;
        seq_t candidate = (seq_t)candidate_idx;   /* wrap if too large for seq_t */

        /* Test for primality. */
        if (is_prime_big(candidate))
            cache[cache_len++] = candidate;

        ++next_k;
    }
}

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;          /* undefined for n=0, but safe guard */
    ensure_cache(n);
    return cache[n - 1];
}
