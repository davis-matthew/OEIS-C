/*
 * A051090  Primes p such that x^42 = -2 has a solution mod p.
 * https://oeis.org/A051090
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   primes p such that the congruence x^42 ≡ -2 (mod p) is solvable
 * Range:     u8 n <= 15, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime_big, mod_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime_big.h"
#include "../utils/mod_pow.h"

static inline idx_t gcd_idx(idx_t a, idx_t b) {
    while (b) {
        idx_t t = b;
        b = a % b;
        a = t;
    }
    return a;
}

seq_t a(idx_t n) {
    /* Static cache of found primes */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;
    static size_t cache_cap = 0;
    static idx_t next_candidate = 2;

    while (cache_len < n) {
        idx_t p = next_candidate;
        /* Skip even candidates > 2 */
        if (p > 2 && (p & 1) == 0) {
            next_candidate++;
            continue;
        }

        /* Check primality */
        if (!is_prime_big((seq_t)p)) {
            next_candidate++;
            continue;
        }

        bool good = false;
        if (p == 2) {
            /* 0 ≡ -2 (mod 2) is solvable via x = 0 */
            good = true;
        } else {
            /* Check if -2 is a 42nd power residue modulo p */
            idx_t g = gcd_idx(42, p - 1);
            idx_t exp = (p - 1) / g;   /* exponent for the test */
            seq_t target = (seq_t)p - 2; /* -2 modulo p (works for p > 2) */
            seq_t val = mod_pow(target, (uint64_t)exp, (seq_t)p);
            if (val == 1)
                good = true;
        }

        if (good) {
            /* Store the prime in the cache */
            if (cache_len == cache_cap) {
                cache_cap = cache_cap ? cache_cap * 2 : 8;
                cache = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
            }
            cache[cache_len++] = (seq_t)p;
        }

        next_candidate++;
    }
    return cache[n - 1];
}
