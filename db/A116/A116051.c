/*
 * A116051  n+sigma(n)+sigma(sigma(n)) is a semiprime (A001358).
 * https://oeis.org/A116051
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   build_primes_upto, ensure_capacity, sigma_of
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A116051
   a(n) = the n-th integer m such that m + σ(m) + σ(σ(m)) is a semiprime
   (product of two primes, possibly equal). */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include "../utils/sigma_of.h"
#include "../utils/build_primes_upto.h"
#include "../utils/ensure_capacity.h"

/* Global cache of primes up to a fixed limit. */
static uint64_t *prime_list = NULL;
static size_t prime_list_cnt = 0;

static bool is_semiprime_uint64(uint64_t x)
{
    if (x < 4) return false;          /* 4 = 2*2 is the smallest semiprime */
    int cnt = 0;
    for (size_t i = 0; i < prime_list_cnt && prime_list[i] * prime_list[i] <= x; ++i) {
        uint64_t p = prime_list[i];
        while (x % p == 0) {
            x /= p;
            ++cnt;
            if (cnt > 2) return false;
        }
    }
    if (x > 1) ++cnt;                 /* remaining prime factor */
    return cnt == 2;
}

seq_t a(idx_t n)
{
    static uint64_t *results = NULL;
    static size_t results_len = 0;
    static size_t results_cap = 0;
    static idx_t last_checked = 0;
    static bool primes_initialized = false;

    if (n == 0) return 0;             /* undefined for offset 1, but guard */

    /* Initialise primes once. */
    if (!primes_initialized) {
        const idx_t prime_limit = 2000000;   /* generous limit for needed factorisation */
        build_primes_upto(prime_limit, &prime_list, &prime_list_cnt);
        primes_initialized = true;
    }

    /* Extend the cache until we have at least n values. */
    while (results_len < (size_t)n) {
        ++last_checked;
        seq_t m = (seq_t)last_checked;
        seq_t sigma_m = sigma_of((idx_t)last_checked);
        seq_t sigma_sigma_m = sigma_of((idx_t)sigma_m);
        seq_t sum = m + sigma_m + sigma_sigma_m;
        uint64_t sum64 = (uint64_t)sum;     /* assume fits in 64 bits for our n */
        if (is_semiprime_uint64(sum64)) {
            ensure_capacity(results_len + 1, &results_cap,
                            (void **)&results, sizeof(uint64_t));
            results[results_len++] = (uint64_t)last_checked;
        }
    }

    return (seq_t)results[n - 1];
}
