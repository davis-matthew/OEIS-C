/*
 * A167445  Prime numbers ending in the prime number 61.
 * https://oeis.org/A167445
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 119, u32 n <= 120, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime_big, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A167445: primes p such that p ≡ 61 (mod 100) – the primes ending in the
   prime number 61. */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "../utils/is_prime_big.h"
#include "../utils/seq_realloc.h"

seq_t a(idx_t n) {
    /* static cache of primes found so far */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;
    static size_t cache_len = 0;
    static idx_t k = 0;          /* next multiplier: candidate = 61 + 100*k */

    /* generate until we have the n‑th term */
    while (cache_len < (size_t)n) {
        /* candidate that ends in 61 */
        uint64_t cand64 = 61ULL + 100ULL * (uint64_t)k;
        ++k;
        seq_t cand = (seq_t)cand64;

        if (is_prime_big(cand)) {
            if (cache_len == cache_cap) {
                size_t new_cap = (cache_cap == 0) ? 64 : cache_cap * 2;
                cache = seq_realloc(cache, cache_cap, new_cap);
                cache_cap = new_cap;
            }
            cache[cache_len++] = cand;
        }
    }

    return cache[n - 1];
}
