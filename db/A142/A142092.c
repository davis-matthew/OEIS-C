/*
 * A142092  Primes congruent to 24 mod 35.
 * https://oeis.org/A142092
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 271, u32 n <= 161, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime_big, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A142092: primes congruent to 24 (mod 35) */

#include <stdbool.h>
#include <stdlib.h>
#include "../utils/is_prime_big.h"
#include "../utils/seq_realloc.h"

/* Static cache of found primes in this arithmetic progression */
static seq_t *prime_cache = NULL;
static size_t cache_cap   = 0;
static size_t cache_size  = 0;

/* Next candidate number ≡ 24 (mod 35) to test for primality */
static seq_t last_candidate = 24;

/* Ensure enough capacity in the cache */
static inline void ensure_cache_cap(void) {
    if (cache_size >= cache_cap) {
        size_t new_cap = (cache_cap == 0) ? 8 : cache_cap * 2;
        seq_t *tmp = seq_realloc(prime_cache, cache_cap, new_cap);
        if (!tmp) exit(EXIT_FAILURE);
        prime_cache = tmp;
        cache_cap   = new_cap;
    }
}

seq_t a(idx_t n) {
    if (n == 0) return 0;                 /* undefined for n = 0, harmless */

    /* Grow the cache until it contains at least n primes */
    while (cache_size < (size_t)n) {
        if (is_prime_big(last_candidate)) {
            ensure_cache_cap();
            prime_cache[cache_size++] = last_candidate;
        }
        last_candidate += 35;            /* next candidate ≡ 24 (mod 35) */
    }

    return prime_cache[(size_t)n - 1];   /* cache is 0‑based */
}
