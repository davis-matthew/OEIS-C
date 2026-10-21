/*
 * A229270  Numbers k for which k' - k is prime, k' being the arithmetic
 *          derivative of k.
 * https://oeis.org/A229270
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   Numbers k such that k′ − k is prime
 * Range:     u8 none, u16 n <= 127, u32 n <= 500, u64 n <= 500, u128 n <= 500,
 *            big any n
 * Helpers:   _floor_isqrt_u64, ensure_cache, is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime_big.h"      /* bool is_prime_big(seq_t n); */
#include "../utils/_floor_isqrt_u64.h"  /* idx_t _floor_isqrt_u64(idx_t n); */
#include "../utils/ensure_cache.h"      /* void ensure_cache(idx_t n, seq_t **ptr, size_t *cap); */

/* Global list of primes for trial division */
static uint64_t *g_primes     = NULL;
static size_t   g_prime_cnt   = 0;
static size_t   g_prime_cap   = 0;
static uint64_t g_prime_limit = 0;

/* Extend prime list up to at least 'limit' using a simple sieve. */
static void ensure_primes(uint64_t limit)
{
    if (limit <= g_prime_limit) return;

    size_t n = limit + 1;
    bool *is_prime = (bool *)malloc(n);
    if (!is_prime) exit(1);
    for (size_t i = 0; i < n; ++i) is_prime[i] = true;
    is_prime[0] = is_prime[1] = false;

    uint64_t sq = _floor_isqrt_u64(limit);
    for (uint64_t i = 2; i <= sq; ++i)
        if (is_prime[i]) {
            for (uint64_t j = i * i; j <= limit; j += i)
                is_prime[j] = false;
        }

    for (uint64_t i = g_prime_limit + 1; i <= limit; ++i)
        if (is_prime[i]) {
            if (g_prime_cnt == g_prime_cap) {
                size_t new_cap = (g_prime_cap == 0) ? 8 : g_prime_cap << 1;
                uint64_t *tmp = (uint64_t *)realloc(g_primes, new_cap * sizeof(uint64_t));
                if (!tmp) exit(1);
                g_primes = tmp;
                g_prime_cap = new_cap;
            }
            g_primes[g_prime_cnt++] = i;
        }

    g_prime_limit = limit;
    free(is_prime);
}

/* Main sequence function */
seq_t a(idx_t n)
{
    /* Static state for memoisation */
    static bool init = false;
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;
    static idx_t  cache_len = 0;
    static uint64_t last_k = 1;            /* last integer tested */

    if (!init) {
        init = true;
        ensure_cache(1, &cache, &cache_cap);
        cache_len = 0;
    }

    /* Generate terms until we have at least n of them */
    while (cache_len < n) {
        ++last_k;                         /* candidate k */
        uint64_t k = last_k;
        uint64_t temp = k;
        uint64_t sqrtk = _floor_isqrt_u64(k);
        ensure_primes(sqrtk);

        seq_t deriv = 0;                  /* arithmetic derivative of k */

        /* Factorise k using the prime table */
        for (size_t i = 0; i < g_prime_cnt && g_primes[i] <= sqrtk; ++i) {
            uint64_t p = g_primes[i];
            if (temp % p == 0) {
                uint64_t exp = 0;
                while (temp % p == 0) {
                    temp /= p;
                    ++exp;
                }
                deriv += ((seq_t)k) / (seq_t)p * (seq_t)exp;
            }
        }
        if (temp > 1) {   /* remaining prime factor > sqrt(k) */
            deriv += ((seq_t)k) / (seq_t)temp;    /* exponent is 1 */
        }

        seq_t diff = deriv - (seq_t)k;
        if (diff > (seq_t)1 && is_prime_big(diff)) {
            ensure_cache(cache_len + 1, &cache, &cache_cap);
            cache[cache_len++] = (seq_t)k;
        }
    }

    return cache[n - 1];   /* a(1) corresponds to cache[0] */
}
