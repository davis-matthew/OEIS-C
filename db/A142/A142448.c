/*
 * A142448  Primes congruent to 40 mod 49.
 * https://oeis.org/A142448
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = the n-th prime p with p ≡ 40 (mod 49)
 * Range:     u8 n <= 1, u16 n <= 152, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* (OEIS sequence A142448) */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* ---------- modular arithmetic for 64‑bit numbers ---------- */

static uint64_t mul_mod(uint64_t a, uint64_t b, uint64_t mod)
{
    return (uint64_t)((__uint128_t)a * b % mod);
}

static uint64_t pow_mod(uint64_t a, uint64_t e, uint64_t mod)
{
    uint64_t r = 1;
    a %= mod;
    while (e) {
        if (e & 1) r = mul_mod(r, a, mod);
        a = mul_mod(a, a, mod);
        e >>= 1;
    }
    return r;
}

/* Deterministic Miller–Rabin for 64‑bit integers.  */
static bool is_prime_u64(uint64_t n)
{
    if (n < 2) return false;
    static const uint64_t small[] = {2,3,5,7,11,13,17,19,23,29,31,37};
    for (size_t i = 0; i < sizeof(small)/sizeof(small[0]); ++i) {
        if (n == small[i]) return true;
        if (n % small[i] == 0) return false;
    }

    uint64_t d = n - 1, s = 0;
    while ((d & 1) == 0) { d >>= 1; ++s; }

    static const uint64_t bases[] =
        {2, 325, 9375, 28178, 450775, 9780504, 1795265022};
    for (size_t i = 0; i < sizeof(bases)/sizeof(bases[0]); ++i) {
        uint64_t a = bases[i] % n;
        if (a == 0) continue;
        uint64_t x = pow_mod(a, d, n);
        if (x == 1 || x == n-1) continue;
        bool cont = false;
        for (uint64_t r = 1; r < s; ++r) {
            x = mul_mod(x, x, n);
            if (x == n-1) { cont = true; break; }
        }
        if (!cont) return false;
    }
    return true;
}

/* ---------- cache for computed primes ---------- */

static seq_t *cache = NULL;      /* array of collected primes   */
static size_t cache_len = 0;     /* number of primes stored     */
static size_t cache_cap = 0;     /* allocated capacity of cache */
static uint64_t next_candidate = 40;   /* next number ≡ 40 (mod 49) */

/* Ensure the cache can hold at least `need` primes. */
static void grow_cache(size_t need)
{
    if (cache_cap >= need) return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 32;
    while (new_cap < need) new_cap *= 2;
    seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
    cache = tmp;
    cache_cap = new_cap;
}

/* Fill the cache with at least `n` terms, starting from where we left off.
   The first time this function is called it also prepares the cache.   */
static void generate_to(idx_t n)
{
    /* initialise on first use */
    if (!cache) {
        cache_cap = 32;
        cache = (seq_t*)malloc(cache_cap * sizeof(seq_t));
        cache_len = 0;
        next_candidate = 40;
    }

    while (cache_len < n) {
        while (!is_prime_u64(next_candidate))
            next_candidate += 49;
        /* store the prime */
        if (cache_len >= cache_cap) grow_cache(cache_len + 2);
        cache[cache_len++] = (seq_t)next_candidate;
        next_candidate += 49;
    }
}

/* ---------- public sequence function ---------- */

seq_t a(idx_t n)
{
    if (cache_len < n) generate_to(n);
    return cache[n - 1];
}
