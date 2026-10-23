/*
 * A142808  Primes congruent to 10 mod 61.
 * https://oeis.org/A142808
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 108, u32 n <= 41, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   grow_seq, is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) = the n-th prime with p ≡ 10 (mod 61) */
#include <stdbool.h>
#include <stddef.h>
#include "../utils/is_prime_big.h"
#include "../utils/grow_seq.h"

seq_t a(idx_t n) {
    /* static cache of found primes */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;
    static size_t cache_cap = 0;
    static idx_t next_k = 1; /* next k to test: p = 10 + 61*k */

    /* Expand cache until we have at least n primes */
    while (cache_len < (size_t)n) {
        /* candidate prime: 10 + 61*k */
        seq_t candidate = 10 + 61 * (seq_t)next_k;
        next_k++;

        if (is_prime_big(candidate)) {
            grow_seq(&cache, &cache_cap, cache_len + 1);
            cache[cache_len++] = candidate;
        }
    }

    return cache[n - 1];
}
