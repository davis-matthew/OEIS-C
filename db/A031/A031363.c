/*
 * A031363  Positive numbers of the form x^2 + xy - y^2; or, of the form 5x^2
 *          - y^2.
 * https://oeis.org/A031363
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   not stated
 * Range:     u8 n <= 64, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   build_primes_upto
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) = the n-th positive integer all whose prime factors ≡ 2 or 3 (mod 5) appear with an even exponent.
   Equivalently, the integers that can be written as x² + xy – y² or 5x² – y². */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <limits.h>

#include "../utils/build_primes_upto.h"

static uint64_t isqrt_u64(uint64_t n)
{
    uint64_t x = 0, b = 1ULL << 62;
    while (b) {
        uint64_t t = x + b;
        if (t && n / t >= t)
            x = t;
        b >>= 1;
    }
    return x;
}

static uint64_t *prime_list = NULL;
static size_t prime_cnt   = 0;

static void ensure_primes(uint64_t limit)
{
    if (prime_cnt && prime_list[prime_cnt - 1] >= limit)
        return;
    if (prime_list) free(prime_list);
    build_primes_upto(limit, &prime_list, &prime_cnt);
}

static bool accept_candidate(uint64_t n)
{
    if (n <= 1) return true;   /* 1 is accepted */

    ensure_primes(isqrt_u64(n));

    uint64_t m = n;
    for (size_t i = 0; i < prime_cnt; ++i) {
        uint64_t p = prime_list[i];
        if (p * p > m) break;
        if (m % p == 0) {
            unsigned exp = 0;
            while (m % p == 0) {
                m /= p;
                ++exp;
            }
            if (((p % 5 == 2) || (p % 5 == 3)) && (exp & 1))
                return false;
        }
    }
    if (m > 1 && ((m % 5 == 2) || (m % 5 == 3)))
        return false;
    return true;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;

    static seq_t *cache      = NULL;
    static size_t cache_sz   = 0;
    static size_t cache_cap  = 0;
    static uint64_t cand     = 1;

    while (cache_sz < (size_t)n) {
        while (!accept_candidate(cand))
            ++cand;

        if (cache_sz == cache_cap) {
            size_t new_cap = (cache_cap == 0) ? 16 : cache_cap * 2;
            seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            if (!tmp) {
                free(cache);
                cache = NULL;
                cache_sz = 0;
                cache_cap = 0;
                exit(EXIT_FAILURE);
            }
            cache = tmp;
            cache_cap = new_cap;
        }
        cache[cache_sz++] = (seq_t)cand;
        ++cand;
    }
    return cache[n - 1];
}
