/*
 * A174242  Least of 3 consecutive integers such that product of them is
 *          average of twin prime pairs.
 * https://oeis.org/A174242
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the least integer k such that k·(k+1)·(k+2)-1 and *
 *            k·(k+1)·(k+2)+1 are both prime. The function searches for such k
 *            and * caches the results
 * Range:     u8 n <= 20, u16 n <= 52, u32 n <= 52, u64 n <= 52, u128 n <= 52,
 *            big any n
 * Helpers:   mul_mod64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/mul_mod64.h"   /* provides mul_mod64(uint64_t, uint64_t, uint64_t) */

/* Helper: modular exponentiation for 64‑bit numbers */
static uint64_t mod_pow_uint64(uint64_t a, uint64_t e, uint64_t mod)
{
    uint64_t res = 1 % mod;
    a %= mod;
    while (e) {
        if (e & 1ULL) res = mul_mod64(res, a, mod);
        a = mul_mod64(a, a, mod);
        e >>= 1ULL;
    }
    return res;
}

/* Helper: deterministic Miller–Rabin primality test for 64‑bit integers */
static bool is_prime_uint64(uint64_t n)
{
    if (n < 2) return false;
    /* small prime trial division */
    static const uint64_t small_primes[] = {
        2ULL,3ULL,5ULL,7ULL,11ULL,13ULL,17ULL,19ULL,23ULL,29ULL,
        31ULL,37ULL,41ULL,43ULL,47ULL,53ULL,59ULL,61ULL,67ULL,71ULL,
        73ULL,79ULL,83ULL,89ULL,97ULL
    };
    for (size_t i = 0; i < sizeof(small_primes)/sizeof(small_primes[0]); ++i) {
        uint64_t p = small_primes[i];
        if (n % p == 0) return n == p;
    }

    /* write n-1 as d * 2^s with d odd */
    uint64_t d = n - 1;
    uint64_t s = 0;
    while ((d & 1ULL) == 0ULL) {
        d >>= 1ULL;
        ++s;
    }

    /* bases sufficient for deterministic test up to 2^64 */
    static const uint64_t bases[] = {
        2ULL, 325ULL, 9375ULL, 28178ULL, 450775ULL, 9780504ULL, 1795265022ULL
    };
    for (size_t i = 0; i < sizeof(bases)/sizeof(bases[0]); ++i) {
        uint64_t a = bases[i] % n;
        if (a == 0) continue;
        uint64_t x = mod_pow_uint64(a, d, n);
        if (x == 1 || x == n - 1) continue;
        bool cont = false;
        for (uint64_t r = 1; r < s; ++r) {
            x = mul_mod64(x, x, n);
            if (x == n - 1) {
                cont = true;
                break;
            }
        }
        if (!cont) return false;
    }
    return true;
}

/* Dynamic cache for discovered terms */
static seq_t *cache = NULL;
static size_t cache_len = 0;
static size_t cache_cap = 0;
static idx_t next_k = 1;       /* next k to test */

/* Ensure the cache can hold at least 'need' elements */
static void ensure_cache(size_t need)
{
    if (need <= cache_cap) return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 64;
    while (new_cap < need) new_cap *= 2;
    seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!new_cache) exit(EXIT_FAILURE);   /* out of memory */
    cache = new_cache;
    cache_cap = new_cap;
}

/* The function required by the problem statement */
seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* offset is 1; values below not used */

    /* Generate terms until we have at least n of them */
    while (cache_len < n) {
        /* Test next_k and increment after */
        uint64_t kk = (uint64_t)next_k;
        uint64_t m  = kk * (kk + 1ULL) * (kk + 2ULL);
        if (is_prime_uint64(m - 1ULL) && is_prime_uint64(m + 1ULL)) {
            ensure_cache(cache_len + 1);
            cache[cache_len++] = (seq_t)next_k;
        }
        ++next_k;
    }
    return cache[n - 1];
}
