/*
 * A089161  Numbers k such that 30*k + 17 and 30*k + 19 are twin primes.
 * https://oeis.org/A089161
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 52, u16 n <= 4928, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   mod_mul
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation of OEIS A089161
   Sequence: numbers k such that 30*k + 17 and 30*k + 19 are twin primes.
   The function a(idx_t n) returns the n‑th such k (a(1)=0).
   It searches k sequentially, tests both numbers with a deterministic
   Miller–Rabin algorithm for 64‑bit integers, and caches found terms.
   This guarantees correct results for any n ≥ 1 (up to the limits
   of the data types involved). */

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/mod_mul.h"

/* ---------- Miller–Rabin for 64‑bit integers ---------- */

static uint64_t mod_pow(uint64_t base, uint64_t exp, uint64_t mod)
{
    uint64_t result = 1;
    uint64_t cur = base % mod;
    while (exp) {
        if (exp & 1) result = mod_mul(result, cur, mod);
        cur = mod_mul(cur, cur, mod);
        exp >>= 1;
    }
    return result;
}

static bool is_prime_u64(uint64_t n)
{
    if (n < 2)                 return false;
    if (n % 2 == 0)             return n == 2;
    if (n % 3 == 0)             return n == 3;
    if (n % 5 == 0)             return n == 5;

    /* small prime trial division */
    static const uint64_t small_primes[] = {
        7,11,13,17,19,23,29,31,37,41,43,47,
        53,59,61,67,71,73,79,83,89,97
    };
    for (size_t i = 0; i < sizeof small_primes / sizeof *small_primes; ++i) {
        uint64_t p = small_primes[i];
        if (n % p == 0) return n == p;
    }

    /* write n-1 as d*2^s */
    uint64_t d = n - 1;
    int s = 0;
    while ((d & 1) == 0) {
        d >>= 1;
        ++s;
    }

    /* deterministic bases for 64‑bit integers */
    static const uint64_t bases[] = {
        2, 325, 9375, 28178, 450775, 9780504, 1795265022
    };
    for (size_t i = 0; i < sizeof bases / sizeof *bases; ++i) {
        uint64_t a = bases[i] % n;
        if (a == 0) continue;
        uint64_t x = mod_pow(a, d, n);
        if (x == 1 || x == n-1) continue;
        bool composite = true;
        for (int r = 1; r < s; ++r) {
            x = mod_mul(x, x, n);
            if (x == n-1) { composite = false; break; }
        }
        if (composite) return false;
    }
    return true;
}

/* ---------- Cached sequence storage ---------- */
static seq_t *cache = NULL;
static size_t cache_len = 0;
static size_t cache_cap = 0;
static idx_t next_k = 0;

/* Append a new term to the cache */
static void cache_append(seq_t val)
{
    if (cache_len == cache_cap) {
        size_t new_cap = (cache_cap == 0) ? 32 : cache_cap * 2;
        seq_t *newp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!newp) abort();  /* memory exhausted */
        cache = newp;
        cache_cap = new_cap;
    }
    cache[cache_len++] = val;
}

/* ---------- Main sequence function ---------- */
seq_t a(idx_t n)
{
    if (n == 0) return 0;  /* defensive; OEIS offset is 1 */

    /* Extend cache until it contains at least n terms */
    while (cache_len < n) {
        idx_t k = next_k;
        /* Compute 30*k+17 and +19 as 64‑bit integers */
        uint64_t p1 = (uint64_t)30 * (uint64_t)k + 17;
        uint64_t p2 = p1 + 2;
        if (is_prime_u64(p1) && is_prime_u64(p2)) {
            cache_append((seq_t)k);
        }
        ++next_k;
    }

    return cache[n - 1];
}
