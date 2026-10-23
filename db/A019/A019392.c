/*
 * A019392  Primes with primitive root 69.
 * https://oeis.org/A019392
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 20, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   _smallest_prime_factor, is_prime, mod_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A prime p has 69 as a primitive root iff
   69 is coprime to p and the multiplicative order of 69 modulo p
   is p-1, i.e. for every prime divisor q of p-1,
   69^((p-1)/q) mod p ≠ 1.
   This function returns the n‑th prime with that property
   (OEIS offset 1). */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/is_prime.h"
#include "../utils/_smallest_prime_factor.h"
#include "../utils/mod_pow.h"

seq_t a(idx_t n)
{
    /* Cache of already found primes. */
    static seq_t *cache     = NULL;
    static size_t cache_len = 0, cache_cap = 0;

    /* Guard for calls with n < 1. */
    if (n == 0) return 0;

    /* Return cached result if already computed. */
    if ((idx_t)cache_len >= n) {
        return cache[n-1];
    }

    /* Start searching from the number after the last cached prime. */
    uint64_t last = cache_len ? (uint64_t)cache[cache_len-1] : 1;

    /* 69 has prime divisors 3 and 23, so skip those primes. */
    const uint64_t skip1 = 3, skip2 = 23;

    for (uint64_t cand = last + 1; ; ++cand) {
        /* Skip 1 and even numbers > 2.  Prime test handles the rest. */
        if (cand == 1) continue;
        if (cand == 2 || is_prime((idx_t)cand)) {

            /* 69 must be coprime to p.  Skip primes dividing 69. */
            if (cand == skip1 || cand == skip2) continue;

            bool primitive = true;

            /* For p = 2 the order is trivially 1. */
            if (cand != 2) {
                uint64_t tmp = cand - 1;
                /* Factor p-1 and test each distinct prime factor. */
                while (tmp > 1 && primitive) {
                    uint64_t spf = _smallest_prime_factor(tmp);
                    if (spf == 0) spf = tmp;          /* fallback */
                    seq_t res = mod_pow((seq_t)69,
                                        (cand-1)/spf,
                                        (seq_t)cand);
                    if (res == (seq_t)1) {
                        primitive = false;
                        break;
                    }
                    while (tmp % spf == 0) tmp /= spf;
                }
            }

            if (primitive) {
                /* Grow cache if necessary. */
                if (cache_len == cache_cap) {
                    cache_cap = cache_cap ? cache_cap * 2 : 16;
                    /* realloc may return NULL if size is 0.  Safeguard. */
                    seq_t *new_cache = (seq_t *)realloc(cache,
                                                        cache_cap * sizeof(seq_t));
                    if (!new_cache) {
                        /* Out of memory – abort. */
                        abort();
                    }
                    cache = new_cache;
                }
                cache[cache_len++] = (seq_t)cand;
                if ((idx_t)cache_len >= n) {
                    return cache[n-1];
                }
            }
        }
    }

    /* Unreachable but keeps compiler happy. */
    return 0;
}
