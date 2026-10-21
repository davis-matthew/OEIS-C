/*
 * A061754  Numbers k such that k! is divisible by (k+1)^6.
 * https://oeis.org/A061754
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 100, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   legendre_valuation
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes a(n) as the nth integer k satisfying k! divisible by (k+1)^6. */
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/legendre_valuation.h"

/* ---------- Prime factorization support ---------- */
static uint64_t *primes = NULL;
static size_t primes_cnt = 0;
static size_t primes_cap = 0;

static void ensure_primes_up_to(uint64_t limit)
{
    if (limit == 0) return;
    if (primes_cnt > 0 && primes[primes_cnt - 1] >= limit) return;

    /* Allocate an initial buffer for primes. */
    if (primes_cap == 0) {
        primes_cap = 10;
        primes = (uint64_t *)malloc(primes_cap * sizeof(uint64_t));
    }

    size_t start = primes_cnt ? primes[primes_cnt - 1] + 1 : 2;
    for (uint64_t cand = start; cand <= limit; ++cand) {
        bool is_prime = true;
        for (size_t i = 0; i < primes_cnt && primes[i] * primes[i] <= cand; ++i) {
            if (cand % primes[i] == 0) { is_prime = false; break; }
        }
        if (is_prime) {
            if (primes_cnt == primes_cap) {
                size_t new_cap = primes_cap * 2;
                uint64_t *tmp = (uint64_t *)realloc(primes, new_cap * sizeof(uint64_t));
                if (!tmp) exit(EXIT_FAILURE);
                primes = tmp;
                primes_cap = new_cap;
            }
            primes[primes_cnt++] = cand;
        }
    }
}

/* Does k! contain (k+1)^6? */
static bool satisfies(idx_t k)
{
    uint64_t m = (uint64_t)k + 1;
    if (m <= 1) return false;
    uint64_t rem = m;

    /* Make sure we have all needed primes for factorisation. */
    ensure_primes_up_to(rem);

    for (size_t i = 0; i < primes_cnt && primes[i] * primes[i] <= rem; ++i) {
        uint64_t p = primes[i];
        if (rem % p != 0) continue;

        unsigned e = 0;
        while (rem % p == 0) { ++e; rem /= p; }

        uint64_t v = legendre_valuation(k, p);
        if (v < (uint64_t)6 * e) return false;
    }
    if (rem > 1) {             /* remaining prime factor */
        uint64_t p = rem;
        unsigned e = 1;
        uint64_t v = legendre_valuation(k, p);
        if (v < (uint64_t)6 * e) return false;
    }
    return true;
}

/* ---------- Sequence computation with memoisation ---------- */
static seq_t *cache = NULL;
static size_t cache_cnt = 0;
static size_t cache_cap = 0;
static idx_t last_k = 0;

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* offset 0 is undefined */

    while (cache_cnt < (size_t)n) {
        ++last_k;
        if (satisfies(last_k)) {
            if (cache_cnt == cache_cap) {
                size_t new_cap = cache_cap ? cache_cap * 2 : 64;
                seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                if (!tmp) exit(EXIT_FAILURE);
                cache = tmp;
                cache_cap = new_cap;
            }
            cache[cache_cnt++] = (seq_t)last_k;
        }
    }
    return cache[n - 1];
}
