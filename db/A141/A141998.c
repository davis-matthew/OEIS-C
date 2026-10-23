/*
 * A141998  Primes congruent to 22 mod 29.
 * https://oeis.org/A141998
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 236, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search: primes congruent to 22 (mod 29) */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/is_prime.h"

static idx_t *prime_cache = NULL;   /* array of found primes (uint64_t) */
static size_t cache_size = 0;       /* number of primes stored */
static size_t cache_cap   = 0;       /* capacity of the array      */
static idx_t next_candidate = 22;   /* candidate ≡ 22 (mod 29) */

/* grow the cache array to accommodate at least 'required' primes */
static void grow_cache(size_t required)
{
    if (cache_cap >= required)
        return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 16;
    while (new_cap < required)
        new_cap *= 2;
    idx_t *new_buf = (idx_t *)realloc(prime_cache, new_cap * sizeof(idx_t));
    if (!new_buf)
        abort();   /* allocation failure */
    prime_cache = new_buf;
    cache_cap = new_cap;
}

/* Return the n‑th prime congruent to 22 (mod 29); n ≥ 1. */
seq_t a(idx_t n)
{
    if (n == 0)           /* undefined for n = 0; return 0 safely */
        return 0;

    while (cache_size < (size_t)n) {
        grow_cache((size_t)n);
        if (is_prime(next_candidate))
            prime_cache[cache_size++] = next_candidate;
        next_candidate += 29;   /* keep the congruence 22 (mod 29) */
    }
    return (seq_t)prime_cache[(size_t)n - 1];
}
