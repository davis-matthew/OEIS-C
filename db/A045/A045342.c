/*
 * A045342  Primes congruent to {1, 2, 3} mod 7.
 * https://oeis.org/A045342
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   primes congruent to 1, 2, or 3 mod 7
 * Range:     u8 n <= 27, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) is the n-th prime p such that p mod 7 ∈ {1,2,3}. */

#include <stdlib.h>
#include "../utils/is_prime_big.h"

static seq_t *cache = NULL;      /* dynamically growable array of found primes */
static size_t cache_cap = 0;     /* current capacity of cache */
static size_t cache_size = 0;    /* number of primes stored in cache */
static idx_t next_candidate = 2; /* next integer to test for primality */

/* appends a prime to the cache, resizing if needed */
static void append_prime(seq_t prime) {
    if (cache_size == cache_cap) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 16;
        seq_t *new_cache = (seq_t *)malloc(sizeof(seq_t) * new_cap);
        if (!new_cache) return; /* memory failure – silent loss, unlikely for n≈10k */
        for (size_t i = 0; i < cache_size; ++i) new_cache[i] = cache[i];
        free(cache);
        cache = new_cache;
        cache_cap = new_cap;
    }
    cache[cache_size++] = prime;
}

/* returns the n-th prime p ≡ 1,2,3 (mod 7), with a(1)=2 */
seq_t a(idx_t n) {
    if (n == 0) return 0; /* undefined offset, but harmless */

    /* generate primes until we have at least n of them */
    while (cache_size < n) {
        seq_t cand = (seq_t)next_candidate;
        if (next_candidate % 7 == 1 || next_candidate % 7 == 2 || next_candidate % 7 == 3) {
            if (is_prime_big(cand))
                append_prime(cand);
        }
        next_candidate++;
    }

    return cache[n - 1];
}
