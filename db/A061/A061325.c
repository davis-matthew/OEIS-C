/*
 * A061325  Primes with 12 as smallest positive primitive root.
 * https://oeis.org/A061325
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 23, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   factor_distinct_uint64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A061325: primes with 12 as smallest positive primitive root.
    The function enumerates primes in increasing order, checks whether
    12 is a primitive root modulo the prime, and verifies that no
    smaller positive integer is a primitive root.  Results are cached
    in a static array so that calls with arbitrary n are fast and
    independent of call order.  */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/factor_distinct_uint64.h"

/* ---------- modular exponentiation (unsigned __int128 for safety) ---------- */
static uint64_t mod_pow_uint64(uint64_t base, uint64_t exp, uint64_t mod)
{
    uint64_t res = 1 % mod;
    base %= mod;
    while (exp) {
        if (exp & 1) res = (unsigned __int128)res * base % mod;
        base = (unsigned __int128)base * base % mod;
        exp >>= 1;
    }
    return res;
}

/* ---------- primality test (trial division) ---------- */
static bool is_prime_uint64(uint64_t n)
{
    if (n < 2) return false;
    if (n == 2) return true;
    if ((n & 1) == 0) return false;
    for (uint64_t d = 3; d * d <= n; d += 2)
        if (n % d == 0) return false;
    return true;
}

/* ---------- primitive‑root test (g modulo prime p) ---------- */
static bool is_primitive_root_uint64(uint64_t g, uint64_t p)
{
    if (g % p == 0) return false;
    uint64_t fac[32]; size_t fc;
    factor_distinct_uint64(p - 1, fac, &fc);
    for (size_t i = 0; i < fc; ++i) {
        if (mod_pow_uint64(g, (p - 1) / fac[i], p) == 1) return false;
    }
    return true;
}

/* ---------- cache of computed terms ---------- */
static uint64_t *cache = NULL;
static size_t cache_len = 0;
static size_t cache_cap = 0;

static void ensure_cache_cap(size_t needed)
{
    if (cache_cap >= needed) return;
    if (cache_cap == 0) cache_cap = 64;
    while (cache_cap < needed) cache_cap <<= 1;
    uint64_t *tmp = (uint64_t *)realloc(cache, cache_cap * sizeof(uint64_t));
    if (!tmp) { free(cache); cache = NULL; cache_cap = 0; } else cache = tmp;
}

/* ---------- main sequence function ---------- */
seq_t a(idx_t n)
{
    if (cache == NULL) {
        cache_cap = 0; cache_len = 0;
        cache = NULL;
    }

    /* generate terms until we have n of them (index 1..n) */
    while (cache_len < (size_t)n) {
        /* candidate prime */
        uint64_t cand;
        if (cache_len == 0) cand = 3;          /* start from the first odd prime */
        else cand = cache[cache_len - 1] + 1;   /* next integer after last prime found */

        if ((cand & 1) == 0) cand++;            /* ensure odd */

        for (;;) {
            if (!is_prime_uint64(cand)) { cand += 2; continue; }

            /* 12 must be a primitive root */
            if (!is_primitive_root_uint64(12, cand)) { cand += 2; continue; }

            /* no smaller positive integer is a primitive root */
            bool bad = false;
            for (uint64_t g = 2; g < 12; ++g) {
                if (is_primitive_root_uint64(g, cand)) { bad = true; break; }
            }
            if (bad) { cand += 2; continue; }

            /* candidate satisfied all conditions */
            ensure_cache_cap(cache_len + 1);
            cache[cache_len++] = cand;
            break;
        }
    }

    return (seq_t)cache[(size_t)n - 1];
}
