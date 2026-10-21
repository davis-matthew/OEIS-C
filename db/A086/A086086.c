/*
 * A086086  Primes p such that p - floor(sqrt(p)) is prime.
 * https://oeis.org/A086086
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the n-th prime p such that p - floor(sqrt(p)) is prime
 *            (computed by searching primes)
 * Range:     u8 n <= 16, u16 n <= 680, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   mod_mul_uint64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/mod_mul_uint64.h"

/* integer square root for 64‑bit numbers */
static inline uint64_t isqrt_u64(uint64_t n)
{
    uint64_t low = 0, high = (uint64_t)1 << 32;      /* sqrt(2^64-1) < 2^32 */
    while (low <= high) {
        uint64_t mid = (low + high) >> 1;
        __uint128_t sq = (__uint128_t)mid * mid;
        if (sq <= n)
            low = mid + 1;
        else {
            if (mid == 0) break;
            high = mid - 1;
        }
    }
    return high;
}

/* modular multiplication using 128‑bit arithmetic */

/* modular exponentiation: a^e mod mod */
static inline uint64_t mod_pow_uint64(uint64_t a, uint64_t e, uint64_t mod)
{
    uint64_t res = 1;
    while (e) {
        if (e & 1)
            res = mod_mul_uint64(res, a, mod);
        a = mod_mul_uint64(a, a, mod);
        e >>= 1;
    }
    return res;
}

/* deterministic Miller–Rabin for 64‑bit integers */
static bool is_prime_u64(uint64_t n)
{
    if (n < 2) return false;
    static const uint64_t small_primes[] = {
        2,3,5,7,11,13,17,19,23,29,31,37
    };
    for (size_t i = 0; i < sizeof(small_primes)/sizeof(small_primes[0]); ++i) {
        uint64_t p = small_primes[i];
        if (n % p == 0) return n == p;
    }
    uint64_t d = n - 1;
    int s = 0;
    while ((d & 1) == 0) {
        d >>= 1;
        ++s;
    }
    static const uint64_t bases[] = {2,3,5,7,11,13,17};
    for (size_t i = 0; i < sizeof(bases)/sizeof(bases[0]); ++i) {
        uint64_t a = bases[i];
        if (a % n == 0) continue;
        uint64_t x = mod_pow_uint64(a, d, n);
        if (x == 1 || x == n - 1) continue;
        bool cont = false;
        for (int r = 1; r < s; ++r) {
            x = mod_mul_uint64(x, x, n);
            if (x == n - 1) {
                cont = true;
                break;
            }
        }
        if (!cont) return false;
    }
    return true;
}

/* memoised sequence storage */
static seq_t *cache      = NULL;
static size_t cache_len  = 0;
static size_t cache_cap  = 0;
static uint64_t next_candidate = 3;   /* next integer to test for primality */

seq_t a(idx_t n)
{
    if (n == 0) return 0;         /* offset is 1; undefined for n==0 */
    while (cache_len < (size_t)n) {
        uint64_t cand = next_candidate;
        next_candidate = cand + 1;          /* prepare for next iteration */
        if (cand < 3) continue;
        if ((cand & 1) == 0) continue;     /* skip even numbers */
        if (!is_prime_u64(cand)) continue;
        uint64_t sqrt_floor = isqrt_u64(cand);
        uint64_t diff = cand - sqrt_floor;
        if (!is_prime_u64(diff)) continue;
        /* store the found prime */
        if (cache_len >= cache_cap) {
            size_t new_cap = cache_cap ? cache_cap * 2 : 8;
            seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            if (!tmp) abort();
            cache = tmp;
            cache_cap = new_cap;
        }
        cache[cache_len++] = (seq_t)cand;
    }
    return cache[(size_t)n - 1];
}
