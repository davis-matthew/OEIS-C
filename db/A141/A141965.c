/*
 * A141965  Primes congruent to 26 mod 27.
 * https://oeis.org/A141965
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 360, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Generates the n‑th prime congruent to 53 (mod 54) by brute‑force search */
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime.h"          /* declares bool is_prime(idx_t) */

seq_t a(idx_t n)
{
    /* static cache of found primes */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;
    static size_t cache_cap = 0;
    static uint64_t last_candidate = 0;   /* last prime that satisfied the congruence */

    /* If we already have enough primes, just return the requested one */
    if (cache_len >= n)
        return cache[n - 1];

    /* Grow the cache as necessary */
    while (cache_len < n) {
        uint64_t candidate;
        if (cache_len == 0) {
            candidate = 53;            /* first prime in the progression */
        } else {
            candidate = last_candidate + 54; /* advance by the modulus */
        }

        /* Find the next prime in the progression */
        while (true) {
            if (is_prime(candidate)) {
                /* Ensure capacity */
                if (cache_len == cache_cap) {
                    size_t new_cap = (cache_cap ? (cache_cap << 1) : 16);
                    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(*cache));
                    cache = tmp;
                    cache_cap = new_cap;
                }
                /* Store and update state */
                cache[cache_len++] = (seq_t)candidate;
                last_candidate = candidate;
                break;
            }
            candidate += 54;
        }
    }
    return cache[n - 1];
}
