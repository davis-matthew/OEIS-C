/*
 * A053444  x^2 + y^2 mod A053443(n) does not take on value a(n).
 * https://oeis.org/A053444
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = least k >= 0 that is not of the form x^2 + y^2 (mod m),
 *            where m = A053443(n); found by search
 * Range:     u8 n <= 90, u16 n <= 5000, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Validated: standard, pass, 119 terms through n = 4986 (2026-11-07)
 * Helpers:   isqrt_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/isqrt_u64.h"
#include "../utils/mem.h"

static bool A053443_has_bad_square(uint64_t x, const uint64_t *primes, size_t primes_cnt)
{
    uint64_t sqr = isqrt_u64(x);
    for (size_t i = 0; i < primes_cnt && primes[i] <= sqr; ++i) {
        uint64_t p = primes[i];
        if (p == 2 || (p & 3ULL) == 3ULL) {
            uint64_t p2 = p * p;
            if (p2 > 0 && x % p2 == 0) return true;
        }
    }
    return false;
}

#define A053443_PRIME_SIEVE_LIMIT 200000ULL

static void A053443_build_prime_list(const uint64_t **primes, size_t *primes_cnt)
{
    static uint64_t *prime_list = NULL;
    static size_t prime_cnt = 0;

    if (prime_cnt != 0) {
        *primes = prime_list;
        *primes_cnt = prime_cnt;
        return;
    }
    size_t limit = A053443_PRIME_SIEVE_LIMIT;
    uint8_t *is_prime = (uint8_t *)malloc(limit + 1);
    if (!is_prime) abort();
    for (size_t i = 0; i <= limit; ++i) is_prime[i] = 1;
    is_prime[0] = is_prime[1] = 0;
    for (size_t i = 2; i * i <= limit; ++i) {
        if (is_prime[i]) {
            for (size_t j = i * i; j <= limit; j += i)
                is_prime[j] = 0;
        }
    }
    size_t cnt = 0;
    for (size_t i = 2; i <= limit; ++i) if (is_prime[i]) ++cnt;
    prime_list = (uint64_t *)malloc(cnt * sizeof(uint64_t));
    if (!prime_list) abort();
    size_t idx = 0;
    for (size_t i = 2; i <= limit; ++i) if (is_prime[i]) prime_list[idx++] = i;
    free(is_prime);

    prime_cnt = cnt;
    *primes = prime_list;
    *primes_cnt = prime_cnt;
}

static seq_t A053443_a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cache_len = 0;
    static size_t cache_cap = 0;
    static uint64_t last_checked = 0;
    static const uint64_t *prime_list = NULL;
    static size_t prime_cnt = 0;

    if (prime_cnt == 0) {
        A053443_build_prime_list(&prime_list, &prime_cnt);
    }
    if (n == 0) return (seq_t)0;

    while (cache_len < (size_t)n) {
        ++last_checked;
        if (A053443_has_bad_square(last_checked, prime_list, prime_cnt)) {
            if (cache_len == cache_cap) {
                size_t new_cap = cache_cap ? cache_cap * 2 : 128;
                seq_t *new_cache = mem_resize(seq_t, cache, new_cap);
                if (!new_cache) abort();
                cache = new_cache;
                cache_cap = new_cap;
            }
            cache[cache_len++] = (seq_t)last_checked;
        }
    }
    return cache[n - 1];
}

seq_t a(idx_t n)
{
    uint64_t m = (uint64_t)(unsigned long long)A053443_a(n);
    unsigned char *sq = (unsigned char *)calloc((size_t)m, 1);
    uint64_t *list = (uint64_t *)malloc(sizeof(uint64_t) * (size_t)m);
    if (!sq || !list) abort();
    size_t cnt = 0;
    for (uint64_t x = 0; x < m; ++x) {
        uint64_t s = (x * x) % m;
        if (!sq[s]) { sq[s] = 1; list[cnt++] = s; }
    }
    uint64_t ans = 0;
    for (uint64_t k = 0; k < m; ++k) {
        bool found = false;
        for (size_t i = 0; i < cnt; ++i) {
            uint64_t r = (k + m - list[i]) % m;
            if (sq[r]) { found = true; break; }
        }
        if (!found) { ans = k; break; }
    }
    free(sq);
    free(list);
    return (seq_t)ans;
}
