/*
 * A138368  Primes of the form k^2 + 12.
 * https://oeis.org/A138368
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 36, u32 n <= 3756, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the nth prime of the form k² + 12 (OEIS A138368).
   a(1) = 13, a(2) = 37, etc. */
#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include "../utils/is_prime_big.h"

/* fast 64‑bit deterministic Miller–Rabin */
static uint64_t modmul64(uint64_t a, uint64_t b, uint64_t mod)
{
    return (uint64_t)((__uint128_t)a * b % mod);
}

static uint64_t modpow64(uint64_t base, uint64_t exp, uint64_t mod)
{
    uint64_t result = 1;
    uint64_t cur = base % mod;
    while (exp) {
        if (exp & 1ULL)
            result = modmul64(result, cur, mod);
        cur = modmul64(cur, cur, mod);
        exp >>= 1;
    }
    return result;
}

static bool is_prime_u64(uint64_t n)
{
    if (n < 2)
        return false;
    static const uint64_t small[] = {2ULL,3ULL,5ULL,7ULL,11ULL,13ULL,17ULL,19ULL,
                                    23ULL,29ULL,31ULL,37ULL};
    for (size_t i = 0; i < sizeof(small)/sizeof(small[0]); ++i) {
        uint64_t p = small[i];
        if (n % p == 0)
            return n == p;
    }
    uint64_t d = n - 1, s = 0;
    while ((d & 1ULL) == 0ULL) {
        d >>= 1;
        ++s;
    }
    static const uint64_t bases[] = {2ULL, 325ULL, 9375ULL, 28178ULL,
                                     450775ULL, 9780504ULL, 1795265022ULL};
    for (size_t i = 0; i < sizeof(bases)/sizeof(bases[0]); ++i) {
        uint64_t a = bases[i] % n;
        if (a == 0)
            continue;
        uint64_t x = modpow64(a, d, n);
        if (x == 1ULL || x == n - 1)
            continue;
        bool composite = true;
        for (uint64_t r = 1; r < s; ++r) {
            x = modmul64(x, x, n);
            if (x == n - 1) {
                composite = false;
                break;
            }
        }
        if (composite)
            return false;
    }
    return true;
}

/* global cache for computed terms */
static seq_t *cache      = NULL;
static size_t cache_len  = 0;   /* number of terms computed */
static size_t cache_cap  = 0;   /* capacity of cache array */
static idx_t  next_k     = 1;   /* next k to test */

/* public function required by the build system */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;           /* undefined for 0 (offset 1) */

    /* generate terms until we reach n */
    while (cache_len < n) {
        /* skip k that cannot possibly give a prime */
        while (!(next_k % 6 == 1 || next_k % 6 == 5))
            ++next_k;

        /* compute candidate value k^2 + 12 */
        seq_t val = (seq_t)next_k * (seq_t)next_k + (seq_t)12;

        bool prime;
        if (sizeof(seq_t) <= sizeof(uint64_t)) {
            uint64_t cand = (uint64_t)val;
            prime = is_prime_u64(cand);
        } else {
            prime = is_prime_big(val);   /* fallback for >64‑bit types */
        }

        if (prime) {
            /* grow cache if necessary */
            if (cache_len >= cache_cap) {
                size_t new_cap = cache_cap ? cache_cap * 2 : 4;
                seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                cache = tmp;
                cache_cap = new_cap;
            }
            cache[cache_len++] = val;
        }

        ++next_k;
    }

    return cache[n - 1];
}
