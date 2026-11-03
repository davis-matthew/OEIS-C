/*
 * A116051  n+sigma(n)+sigma(sigma(n)) is a semiprime (A001358).
 * https://oeis.org/A116051
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 74 terms through n = 910 (2026-11-01)
 * Helpers:   build_primes_upto
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A116051
   a(n) = the n-th integer m such that m + σ(m) + σ(σ(m)) is a semiprime
   (product of two primes, possibly equal). */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/build_primes_upto.h"

/* Compute σ(i) by trial division up to √i. */
static seq_t sigma_of(idx_t n)
{
    seq_t sum = (seq_t)0;
    for (idx_t d = 1; d * d <= n; ++d) {
        if (n % d == 0) {
            sum += (seq_t)d;
            idx_t d2 = n / d;
            if (d2 != d)
                sum += (seq_t)d2;
        }
    }
    return sum;
}

/* Grow a dynamic array when needed.  The pointer is passed as (void **). */
static void ensure_capacity(size_t needed, size_t *cap, void **ptr, size_t elem_size)
{
    if (needed <= *cap) return;
    size_t new_cap = (*cap == 0) ? 4 : (*cap << 1);
    while (new_cap < needed) new_cap <<= 1;
    void *tmp = realloc(*ptr, new_cap * elem_size);
    if (!tmp) abort();               /* on OOM we abort as required */
    *ptr = tmp;
    *cap = new_cap;
}

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
