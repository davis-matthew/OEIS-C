/*
 * A105133  Numbers n such that 8n + 5 is prime.
 * https://oeis.org/A105133
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 80, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A105133: numbers n such that 8*n + 5 is prime.
   a(n) = (prime(n) - 5) / 8.
   The implementation generates the terms on demand and caches them
   for repeated calls. */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* 64‑bit modular multiplication using 128‑bit arithmetic. */
static uint64_t mulmod64(uint64_t a, uint64_t b, uint64_t mod)
{
    return (uint64_t)(((__uint128_t)a * b) % mod);
}

/* 64‑bit modular exponentiation. */
static uint64_t modpow64(uint64_t base, uint64_t exp, uint64_t mod)
{
    uint64_t result = 1;
    base %= mod;
    while (exp != 0)
    {
        if (exp & 1)
            result = mulmod64(result, base, mod);
        base = mulmod64(base, base, mod);
        exp >>= 1;
    }
    return result;
}

/* Deterministic Miller–Rabin primality test for 64‑bit unsigned integers. */
static bool is_prime_64(uint64_t n)
{
    if (n < 2)
        return false;
    /* Check small primes first. */
    static const uint64_t small[] = {2,3,5,7,11,13,17,19,23,29,31,37};
    for (size_t i = 0; i < sizeof(small)/sizeof(small[0]); ++i)
    {
        uint64_t p = small[i];
        if (n % p == 0)
            return n == p;
    }

    /* Write n-1 as d * 2^s with d odd. */
    uint64_t d = n - 1;
    uint64_t s = 0;
    while ((d & 1) == 0)
    {
        d >>= 1;
        ++s;
    }

    /* Deterministic bases for 64-bit integers. */
    static const uint64_t bases[] = {2, 325, 9375, 28178, 450775, 9780504, 1795265022};
    for (size_t i = 0; i < sizeof(bases)/sizeof(bases[0]); ++i)
    {
        uint64_t a = bases[i] % n;
        if (a == 0) continue;
        uint64_t x = modpow64(a, d, n);
        if (x == 1 || x == n - 1)
            continue;
        bool composite = true;
        for (uint64_t r = 1; r < s; ++r)
        {
            x = mulmod64(x, x, n);
            if (x == n - 1)
            {
                composite = false;
                break;
            }
        }
        if (composite)
            return false;
    }
    return true;
}

/* ---------- Caching machinery ---------- */
static seq_t *cache = NULL;
static idx_t cache_capacity = 0;
static idx_t cache_size = 0;
static idx_t next_test = 0;          /* next n (0‑based) to test for primality */

/* Ensure the cache can hold at least `needed` elements. */
static void ensure_capacity(idx_t needed)
{
    if (needed <= cache_capacity)
        return;
    idx_t new_cap = cache_capacity ? cache_capacity << 1 : 128;
    if (new_cap < needed) new_cap = needed;
    seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!new_cache)
        abort();    /* out of memory */
    cache = new_cache;
    cache_capacity = new_cap;
}

/* Generate terms until we have at least `target` terms in the cache. */
static void generate_until(idx_t target)
{
    if (target <= cache_size)
        return;
    ensure_capacity(target);
    while (cache_size < target)
    {
        /* candidate n = next_test (0‑based).  a(1) corresponds to n = 0. */
        uint64_t cand = next_test;
        uint64_t value = 8 * cand + 5;
        if (is_prime_64(value))
        {
            cache[cache_size] = (seq_t)cand;   /* store a(n) = cand */
            ++cache_size;
        }
        ++next_test;
    }
}

/* ---------- Public function ---------- */
seq_t a(idx_t n)
{
    if (n == 0)          /* offset is 1, but be safe for n == 0 */
        return 0;
    generate_until(n);
    return cache[n - 1];
}
