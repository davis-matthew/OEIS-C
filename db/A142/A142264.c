/*
 * A142264  Primes congruent to 15 mod 43.
 * https://oeis.org/A142264
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = the nth prime congruent to 15 (mod 43)
 * Range:     u8 n <= 1, u16 n <= 148, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

static const size_t SMALL_PRIME_LIMIT = 1000000;
static const size_t SMALL_PRIME_CAP = 80000;
static uint32_t *small_primes = NULL;
static size_t small_primes_cnt = 0;
static bool small_primes_inited = false;

/* Build a list of all primes up to SMALL_PRIME_LIMIT using a simple sieve */
static void init_small_primes(void) {
    if (small_primes_inited) return;
    bool *is_comp = (bool*)malloc(SMALL_PRIME_LIMIT + 1);
    if (!is_comp) exit(EXIT_FAILURE);  /* memory error */
    for (size_t i = 2; i <= SMALL_PRIME_LIMIT; ++i) is_comp[i] = false;

    for (size_t i = 2; i * i <= SMALL_PRIME_LIMIT; ++i) {
        if (!is_comp[i]) {
            for (size_t j = i * i; j <= SMALL_PRIME_LIMIT; j += i)
                is_comp[j] = true;
        }
    }

    small_primes = (uint32_t*)malloc(SMALL_PRIME_CAP * sizeof(uint32_t));
    if (!small_primes) exit(EXIT_FAILURE);
    small_primes_cnt = 0;
    for (size_t i = 2; i <= SMALL_PRIME_LIMIT && small_primes_cnt < SMALL_PRIME_CAP; ++i) {
        if (!is_comp[i]) small_primes[small_primes_cnt++] = (uint32_t)i;
    }
    free(is_comp);
    small_primes_inited = true;
}

/* Test whether x is prime using precomputed small primes */
static bool is_prime_candidate(uint64_t x) {
    if (x < 2) return false;
    for (size_t i = 0; i < small_primes_cnt; ++i) {
        uint32_t p = small_primes[i];
        if ((uint64_t)p * (uint64_t)p > x) break;
        if (x % p == 0) return false;
    }
    return true;
}

/* Cache for computed terms */
static seq_t *cache = NULL;
static size_t cache_len = 0;
static size_t cache_cap = 0;
static size_t candidate_k = 0; /* index of next candidate (value = 43*k + 15) */

seq_t a(idx_t n) {
    if (n == 0) return 0; /* Undefined for index 0, return 0 */

    if (!small_primes_inited) init_small_primes();

    /* Grow cache storage */
    if (!cache_cap) {
        cache_cap = 64;
        cache = (seq_t*)malloc(cache_cap * sizeof(seq_t));
        if (!cache) exit(EXIT_FAILURE);
    }

    /* Generate terms until we have at least n of them */
    while (cache_len < (size_t)n) {
        uint64_t candidate = 43ULL * (uint64_t)candidate_k + 15ULL;
        ++candidate_k;
        if (is_prime_candidate(candidate)) {
            if (cache_len == cache_cap) {
                cache_cap *= 2;
                cache = (seq_t*)realloc(cache, cache_cap * sizeof(seq_t));
                if (!cache) exit(EXIT_FAILURE);
            }
            cache[cache_len++] = (seq_t)candidate;
        }
    }

    return cache[(size_t)n - 1];
}
