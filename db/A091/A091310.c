/*
 * A091310  Primes not of the form p*q + p - q with prime p and q.
 * https://oeis.org/A091310
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   primes not of the form p*q + p - q with prime p and q (OEIS
 *            A091310). a(1) = 2. The function is memoised so that subsequent
 *            calls with arbitrary indices are fast
 * Range:     u8 n <= 22, u16 n <= 3680, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>
#include "../utils/is_prime.h"

seq_t a(idx_t n)
{
    /* static cache of the sequence values */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0, cache_len = 0;

    /* static list of primes found so far (uint64_t) */
    static uint64_t *primes = NULL;
    static size_t primes_cap = 0, primes_len = 0;

    /* last integer examined (inclusive) */
    static uint64_t last_checked = 1;

    /* extend cache until it contains at least n elements */
    while (cache_len < n) {
        uint64_t cand = last_checked + 1U;

        /* test primality of the candidate */
        if (is_prime(cand)) {
            /* check whether cand can be expressed as p*q + p - q */
            bool expressible = false;
            for (size_t i = 0; i < primes_len; ++i) {
                uint64_t q = primes[i];
                /* q must be <= cand-2 */
                if (q > cand - 2U) {
                    break;
                }
                uint64_t num = cand + q;       /* cand + q */
                uint64_t den = q + 1U;         /* q + 1 */
                if (num % den != 0U) {
                    continue;
                }
                uint64_t p = num / den;
                if (p >= 2U && is_prime(p)) {
                    expressible = true;
                    break;
                }
            }
            if (!expressible) {
                /* ensure cache capacity */
                if (cache_len == cache_cap) {
                    size_t new_cap = cache_cap ? cache_cap * 2 : 64;
                    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                    if (!tmp) exit(EXIT_FAILURE);
                    cache = tmp;
                    cache_cap = new_cap;
                }
                /* store the prime in the cache */
                cache[cache_len++] = (seq_t) cand;
            }

            /* store the prime in the primes list */
            if (primes_len == primes_cap) {
                size_t new_cap = primes_cap ? primes_cap * 2 : 64;
                uint64_t *tmp = (uint64_t *)realloc(primes, new_cap * sizeof(uint64_t));
                if (!tmp) exit(EXIT_FAILURE);
                primes = tmp;
                primes_cap = new_cap;
            }
            primes[primes_len++] = cand;
        }

        last_checked = cand;
    }

    return cache[n - 1];
}
