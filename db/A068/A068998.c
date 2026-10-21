/*
 * A068998  Numbers m such that the concatenation of the prime factors of m
 *          (in increasing order and ignoring multiplicity) is prime.
 * https://oeis.org/A068998
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 136, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes A068998: numbers m such that the concatenation of the
   distinct prime factors of m (in increasing order) is a prime number. */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* Helper: deterministic Miller–Rabin for 64‑bit integers. */
static uint64_t mod_mul(uint64_t a, uint64_t b, uint64_t mod)
{
    return (uint64_t)((__uint128_t)a * b % mod);
}

static uint64_t mod_pow(uint64_t a, uint64_t d, uint64_t mod)
{
    uint64_t r = 1;
    while (d) {
        if (d & 1) r = mod_mul(r, a, mod);
        a = mod_mul(a, a, mod);
        d >>= 1;
    }
    return r;
}

static bool is_prime_u64(uint64_t n)
{
    if (n < 2) return false;
    static const uint64_t small_primes[] = {2,3,5,7,11,13,17,19,23,29,31,37};
    for (size_t i = 0; i < sizeof(small_primes)/sizeof(small_primes[0]); ++i) {
        if (n % small_primes[i] == 0) return n == small_primes[i];
    }
    uint64_t d = n - 1, s = 0;
    while ((d & 1) == 0) { d >>= 1; ++s; }

    /* Bases for deterministic test up to 2^64. */
    static const uint64_t bases[] = {
        2, 325, 9375, 28178, 450775, 9780504, 1795265022
    };
    for (size_t i = 0; i < sizeof(bases)/sizeof(bases[0]); ++i) {
        uint64_t a = bases[i] % n;
        if (a == 0) continue;
        uint64_t x = mod_pow(a, d, n);
        if (x == 1 || x == n - 1) continue;
        bool comp = true;
        for (uint64_t r = 1; r < s; ++r) {
            x = mod_mul(x, x, n);
            if (x == n - 1) { comp = false; break; }
        }
        if (comp) return false;
    }
    return true;
}

/* Cache for found terms. */
static seq_t *cache = NULL;
static size_t cache_len = 0, cache_cap = 0;
static idx_t next_m = 2;                    /* next integer to test */

/* Ensure the cache contains at least 'need' terms. */
static void ensure_cache(size_t need)
{
    if (cache_len >= need) return;

    if (!cache) {
        cache_cap = 4096;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
    }

    idx_t m;
    for (m = next_m; cache_len < need; ++m) {
        uint64_t x = (uint64_t)m;
        uint64_t primes[64];
        size_t pcnt = 0;

        /* Factor x, collect distinct primes. */
        if (x % 2 == 0) {
            primes[pcnt++] = 2;
            while (x % 2 == 0) x /= 2;
        }
        for (uint64_t d = 3; d * d <= x; d += 2) {
            if (x % d == 0) {
                primes[pcnt++] = d;
                while (x % d == 0) x /= d;
            }
        }
        if (x > 1) primes[pcnt++] = x;

        /* Concatenate the distinct primes. */
        uint64_t concat = 0;
        for (size_t i = 0; i < pcnt; ++i) {
            uint64_t p = primes[i];
            /* Count digits of p. */
            uint64_t digits = 0, tmp = p;
            do { ++digits; tmp /= 10; } while (tmp);
            /* Compute 10^digits. */
            uint64_t mult = 1;
            for (uint64_t d = 0; d < digits; ++d) mult *= 10U;
            concat = concat * mult + p;
        }

        if (is_prime_u64(concat)) {
            if (cache_len == cache_cap) {
                cache_cap *= 2;
                cache = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
            }
            cache[cache_len++] = (seq_t)m;
        }
    }
    next_m = m;   /* last tested + 1 */
}

/* External function required by the build. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;         /* undefined for offset 0 */
    ensure_cache((size_t)n);
    return cache[(size_t)n - 1];
}
