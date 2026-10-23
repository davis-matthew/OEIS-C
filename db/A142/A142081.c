/*
 * A142081  Primes congruent to 8 mod 35.
 * https://oeis.org/A142081
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 274, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The formula: a(n) is the n-th prime congruent to 8 (mod 35). */
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/is_prime.h"

/* Dynamic cache of found primes. */
static seq_t *prime_cache = NULL;
static size_t cache_len   = 0;   /* number of primes stored */
static size_t cache_cap   = 0;   /* allocated capacity */
static idx_t  last_k      = 0;   /* last k used to generate candidate */

/* Expand cache capacity to hold at least 'needed' elements. */
static void grow_cache(size_t needed)
{
    if (needed <= cache_cap)
        return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 16;
    while (new_cap < needed)
        new_cap *= 2;
    seq_t *new_ptr = (seq_t *)realloc(prime_cache, new_cap * sizeof(seq_t));
    if (!new_ptr)
        exit(EXIT_FAILURE); /* unrecoverable memory error */
    prime_cache = new_ptr;
    cache_cap   = new_cap;
}

/* Return the nth prime ≡ 8 (mod 35).  n is 1‑based; a(1) = 43. */
seq_t a(idx_t n)
{
    if (n <= 0)
        return 0;          /* undefined for n<1, but safe return */

    /* Generate primes until we have at least n in the cache. */
    while (cache_len < n)
    {
        /* Find next candidate of the form 35*k + 8. */
        ++last_k;
        idx_t candidate = 35 * last_k + 8;

        /* Test primality.  is_prime works on idx_t (≤64‑bit). */
        if (is_prime(candidate))
        {
            grow_cache(cache_len + 1);
            prime_cache[cache_len++] = (seq_t)candidate;
        }
    }
    return prime_cache[n - 1];
}
