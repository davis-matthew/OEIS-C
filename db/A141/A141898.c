/*
 * A141898  Primes congruent to 20 mod 21.
 * https://oeis.org/A141898
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 547, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: all primes congruent to 20 modulo 21 (search) */

#include <stdbool.h>
#include <stdlib.h>
#include "../utils/is_prime.h"

/* Memoisation cache for found primes */
static seq_t *cache = NULL;      /* array of primes found so far */
static size_t cache_len = 0;     /* current number of stored primes  */
static size_t cache_cap = 0;     /* current allocated capacity       */

/* Next candidate congruent to 20 mod 21 (initially 41) */
static idx_t next_candidate = 41;

/* Ensure the cache can hold at least 'need' elements */
static void ensure_cache_capacity(size_t need)
{
    if (need <= cache_cap) return;
    size_t new_cap = cache_cap ? cache_cap << 1 : 8;
    if (new_cap < need) new_cap = need;
    seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!new_cache) abort();
    cache = new_cache;
    cache_cap = new_cap;
}

seq_t a(idx_t n)
{
    /* n ≥ 1 by OEIS offset; behaviour for n < 1 is irrelevant */
    while (cache_len < (size_t)n) {
        /* Find the next prime p ≡ 20 (mod 21) */
        while (!is_prime(next_candidate))
            next_candidate += 21;            /* skip to next congruent number */

        /* Store the found prime */
        ensure_cache_capacity(cache_len + 1);
        cache[cache_len++] = (seq_t)next_candidate;

        /* Prepare the next candidate */
        next_candidate += 21;
    }
    return cache[n - 1];
}
