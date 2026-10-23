/*
 * A142519  Primes congruent to 25 mod 52.
 * https://oeis.org/A142519
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) is the n-th prime ≡ 25 (mod 52)
 * Range:     u8 n <= 2, u16 n <= 270, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The algorithm searches candidates of the form 25+52*k,
   tests them for primality with is_prime(idx_t), and caches
   the found primes. The cache grows lazily as larger indices
   are requested, so calls can be in any order. */
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime.h"

static seq_t *prime_cache = NULL;  /* cached primes */
static idx_t   cache_size  = 0;    /* number of cached primes   */
static idx_t   cache_cap   = 0;    /* allocated cache capacity */
static idx_t   next_k      = 0;    /* next k to examine in 25+52*k */

/* Ensure that prime_cache contains at least 'target' primes. */
static void ensure_cache(idx_t target)
{
    if (cache_size >= target)
        return;

    /* Initialise cache if not yet allocated */
    if (!prime_cache) {
        cache_cap = 128;
        prime_cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!prime_cache)
            exit(1);
    }

    while (cache_size < target) {
        /* Find the next candidate that is prime. */
        idx_t candidate;
        for (;;) {
            candidate = 25 + 52 * next_k;
            ++next_k;
            if (is_prime(candidate))
                break;                /* prime found */
        }

        /* Grow cache if necessary. */
        if (cache_size >= cache_cap) {
            cache_cap <<= 1;
            seq_t *tmp = (seq_t *)realloc(prime_cache, cache_cap * sizeof(seq_t));
            if (!tmp)
                exit(1);
            prime_cache = tmp;
        }

        /* Store the found prime. */
        prime_cache[cache_size++] = (seq_t)candidate;
    }
}

seq_t a(idx_t n)
{
    if (n < 1)
        return (seq_t)0;              /* undefined for n < 1 */
    ensure_cache(n);
    return prime_cache[n - 1];
}
