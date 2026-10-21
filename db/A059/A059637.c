/*
 * A059637  Primes p such that x^45 = 2 has no solution mod p.
 * https://oeis.org/A059637
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   Primes p for which the congruence x^45 = 2 has no solution
 *            modulo p. * This is equivalent to: 2^((p‑1)/gcd(45,p‑1)) ≠ 1
 *            (mod p)
 * Range:     u8 n <= 26, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   mod_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/mod_pow.h"

/* Greatest common divisor for 64‑bit unsigned integers. */
static inline uint64_t gcd64(uint64_t a, uint64_t b) {
    while (b) {
        uint64_t t = b;
        b = a % b;
        a = t;
    }
    return a;
}

/* The sequence function. */
seq_t a(idx_t n) {
    /* Static state for caching computed terms and prime generation. */
    static seq_t *cache = NULL;
    static size_t cache_len = 0, cache_cap = 0;

    static uint64_t *prime_list = NULL;
    static size_t prime_len = 0, prime_cap = 0;
    static uint64_t cand = 3;          /* Next candidate for primality test. */

    /* Ensure we have at least n terms in the cache. */
    while (cache_len < (size_t)n) {
        uint64_t p = 0;

        /* Find the next prime > 2 and add it to prime_list. */
        for (;;) {
            if (cand == 2) {
                p = 2;
                /* Still record 2 in the list so future divisions work. */
                if (prime_len == prime_cap) {
                    size_t new_cap = prime_cap ? prime_cap * 2 : 1;
                    uint64_t *tmp = (uint64_t *)realloc(prime_list, new_cap * sizeof(uint64_t));
                    if (!tmp) abort();
                    prime_list = tmp;
                    prime_cap = new_cap;
                }
                prime_list[prime_len++] = cand;
                cand++;
                continue;          /* Skip 2 for the sequence itself. */
            }

            if (cand % 2 == 0) { cand++; continue; }

            bool is_prime = true;
            for (size_t i = 0; i < prime_len; ++i) {
                uint64_t pp = prime_list[i];
                if (pp * pp > cand) break;
                if (cand % pp == 0) { is_prime = false; break; }
            }

            if (is_prime) {
                if (prime_len == prime_cap) {
                    size_t new_cap = prime_cap ? prime_cap * 2 : 1;
                    uint64_t *tmp = (uint64_t *)realloc(prime_list, new_cap * sizeof(uint64_t));
                    if (!tmp) abort();
                    prime_list = tmp;
                    prime_cap = new_cap;
                }
                prime_list[prime_len++] = cand;
                p = cand;
                cand++;
                break;                /* Found next prime. */
            }
            cand++;
        }

        if (p == 2) continue;   /* Sequence begins with primes > 2. */

        uint64_t g = gcd64(45ULL, p - 1);
        uint64_t exp = (p - 1) / g;
        seq_t r = mod_pow((seq_t)2, exp, (seq_t)p);

        if (r != (seq_t)1ULL) {
            if (cache_len == cache_cap) {
                size_t new_cap = cache_cap ? cache_cap * 2 : 1;
                seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                if (!tmp) abort();
                cache = tmp;
                cache_cap = new_cap;
            }
            cache[cache_len++] = (seq_t)p;
        }
    }

    return cache[(size_t)n - 1];
}
