/*
 * A141934  Primes congruent to 9 mod 25.
 * https://oeis.org/A141934
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 321, u32 n <= 322, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the n-th prime congruent to 9 (mod 25) by testing successive
 * candidates with is_prime_big.  It caches the primes found so far so that
 * repeated calls with arbitrary n are efficient. */
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime_big.h"

seq_t a(idx_t n)
{
    /* static cache of primes found so far */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;   /* number of primes stored */
    static size_t cache_cap = 0;   /* allocated capacity */
    static seq_t next_candidate;   /* next number ≡ 9 (mod 25) to test */
    static bool initialized = false;

    if (!initialized) {
        initialized = true;
        cache_cap = 16;
        cache = (seq_t *)malloc(sizeof(seq_t) * cache_cap);
        if (!cache) return (seq_t)0;    /* allocation failed – graceful */
        next_candidate = (seq_t)9;      /* will be incremented to 34 on first test */
    }

    /* Generate primes until we have at least n entries */
    while (cache_len < (size_t)n) {
        /* advance to next 9 (mod 25) number */
        next_candidate += (seq_t)25;

        /* test primality */
        if (is_prime_big(next_candidate)) {
            /* grow cache if necessary */
            if (cache_len == cache_cap) {
                cache_cap *= 2;
                seq_t *tmp = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
                if (!tmp) {
                    free(cache);
                    cache = NULL;
                    return (seq_t)0;   /* allocation failed – graceful */
                }
                cache = tmp;
            }
            cache[cache_len++] = next_candidate;
        }
    }

    return cache[(size_t)n - 1];
}
