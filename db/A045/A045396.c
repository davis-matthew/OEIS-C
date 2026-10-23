/*
 * A045396  Primes congruent to {0, 1, 3, 4, 5} mod 7.
 * https://oeis.org/A045396
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = the n‑th prime that satisfies p ≡ 0,1,3,4 or 5 (mod 7)
 * Range:     u8 n <= 36, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* (OEIS A045396) */
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>

/* static storage for cached sequence elements and known primes */
static seq_t *cache      = NULL;
static size_t cache_len  = 0, cache_cap = 0;
static uint64_t *primes = NULL;
static size_t primes_len = 0, primes_cap = 0;
static uint64_t last_candidate = 2;
static bool  initialized = false;

/* grow slot for seq_t cache */
static void grow_cache(size_t needed) {
    size_t new_cap = cache_cap ? cache_cap : 16;
    while (new_cap < needed) new_cap <<= 1;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp) abort();
    cache  = tmp;
    cache_cap = new_cap;
}

/* grow slot for uint64_t primes list */
static void grow_primes(size_t needed) {
    size_t new_cap = primes_cap ? primes_cap : 16;
    while (new_cap < needed) new_cap <<= 1;
    uint64_t *tmp = (uint64_t *)realloc(primes, new_cap * sizeof(uint64_t));
    if (!tmp) abort();
    primes  = tmp;
    primes_cap = new_cap;
}

/* simple trial‑division primality test using the primes list already built */
static bool is_prime_candidate(uint64_t x) {
    if (x < 2) return false;
    for (size_t i = 0; i < primes_len; ++i) {
        uint64_t p = primes[i];
        if (p > x / p) break;          /* p*p would overflow or > x */
        if (x % p == 0) return false;
    }
    return true;
}

/* expand the cache until it contains at least n terms */
static void ensure_terms(idx_t n) {
    if (cache_len >= (size_t)n) return;

    grow_cache((size_t)n);

    if (!initialized) {
        /* seed the primes list with the first prime (2) */
        grow_primes(1);
        primes[0] = 2;
        primes_len = 1;
        last_candidate = 2;
        initialized = true;
    }

    /* iterate over candidates until we have n cached terms */
    while (cache_len < n) {
        uint64_t cand = ++last_candidate;

        /* skip even numbers greater than 2 */
        if (cand > 2 && (cand & 1ULL) == 0)
            continue;

        if (is_prime_candidate(cand)) {
            /* add to the primes list */
            if (primes_len == primes_cap)
                grow_primes(primes_cap << 1);
            primes[primes_len++] = cand;

            /* check the required residue modulo 7 */
            uint64_t r = cand % 7ULL;
            if (r == 0ULL || r == 1ULL || r == 3ULL || r == 4ULL || r == 5ULL) {
                cache[cache_len++] = (seq_t)cand;
            }
        }
    }
}

seq_t a(idx_t n) {
    /* offset 1: a(1) is the first element */
    /* n is 1‑based */
    ensure_terms(n);
    return cache[(size_t)n - 1];
}
