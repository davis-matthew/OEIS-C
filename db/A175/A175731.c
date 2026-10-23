/*
 * A175731  Semiprimes m such that either 2m-1 or 2m+1 is prime, but not both.
 * https://oeis.org/A175731
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 25, u16 n <= 59, u32 n <= 59, u64 n <= 59, u128 n <= 59,
 *            big any n
 * Helpers:   isPrimeTrial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A175731: semiprimes m such that exactly one of 2m-1 or 2m+1 is prime. */
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/isPrimeTrial.h"

static const uint64_t INITIAL_SPF_LIMIT = 2000000ULL; /* first sieve size */

static uint64_t *spf = NULL;
static uint64_t spf_limit = 0;
static bool spf_built = false;

static uint64_t *cache = NULL;
static size_t cache_len = 0;
static size_t cache_cap = 0;
static uint64_t last_checked = 3; /* last m examined */

/* Grow the cache array storing found semiprimes */
static void grow_cache(void) {
    if (cache_len == cache_cap) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 64;
        uint64_t *tmp = (uint64_t*)realloc(cache, new_cap * sizeof(uint64_t));
        if (!tmp) exit(EXIT_FAILURE);
        cache = tmp;
        cache_cap = new_cap;
    }
}

/* Build smallest‑prime‑factor array up to 'limit' */
static void build_spf_up_to(uint64_t limit) {
    uint64_t *new_spf = (uint64_t*)calloc(limit + 1, sizeof(uint64_t));
    if (!new_spf) exit(EXIT_FAILURE);
    for (uint64_t i = 2; i <= limit; ++i) {
        if (new_spf[i] == 0) {
            new_spf[i] = i;
            if (i * i <= limit) {
                for (uint64_t j = i * i; j <= limit; j += i) {
                    if (new_spf[j] == 0) new_spf[j] = i;
                }
            }
        }
    }
    if (spf) free(spf);
    spf = new_spf;
    spf_limit = limit;
    spf_built = true;
}

/* Ensure the SPF table covers at least 'n'. */
static void ensure_spf(uint64_t n) {
    if (!spf_built) {
        build_spf_up_to(INITIAL_SPF_LIMIT);
    }
    while (n > spf_limit) {
        uint64_t new_limit = spf_limit * 2;
        build_spf_up_to(new_limit);
    }
}

/* Return true iff x is a semiprime (product of exactly two primes). */
static inline bool is_semiprime(uint64_t x) {
    if (x < 4) return false;
    uint64_t p = spf[x];
    if (p == 0 || p == x) return false; /* prime or missing factor */
    uint64_t q = x / p;
    return (q >= 2) && (spf[q] == q);
}

/* Sequence function */
seq_t a(idx_t n) {
    if (n == 0) return 0;
    if (cache_len >= (size_t)n) {
        return (seq_t)cache[n - 1];
    }
    if (!spf_built) build_spf_up_to(INITIAL_SPF_LIMIT);

    while (cache_len < (size_t)n) {
        ++last_checked;
        ensure_spf(last_checked);
        if (is_semiprime(last_checked)) {
            unsigned long long v1 = 2 * last_checked - 1;
            unsigned long long v2 = 2 * last_checked + 1;
            bool p1 = isPrimeTrial((seq_t)v1);
            bool p2 = isPrimeTrial((seq_t)v2);
            if (p1 ^ p2) {
                grow_cache();
                cache[cache_len++] = last_checked;
            }
        }
    }
    return (seq_t)cache[n - 1];
}
