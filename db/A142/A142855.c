/*
 * A142855  Primes congruent to 57 mod 61.
 * https://oeis.org/A142855
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 114, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   mod_mul
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: primes p such that p ≡ 57 (mod 61) (offset 1) */
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/mod_mul.h"

/* Deterministic Miller‑Rabin for 64‑bit unsigned integers.
   Uses the bases {2,3,5,7,11,13,17} which are sufficient for all n < 2^64. */

static inline uint64_t mod_pow(uint64_t a, uint64_t e, uint64_t mod)
{
    uint64_t res = 1;
    while (e)
    {
        if (e & 1ULL)
            res = mod_mul(res, a, mod);
        a = mod_mul(a, a, mod);
        e >>= 1;
    }
    return res;
}

static bool is_prime_uint64(uint64_t n)
{
    if (n < 2) return false;
    /* small primes */
    static const uint64_t smalls[] = {2,3,5,7,11,13,17,19,23,29,31,37};
    for (size_t i = 0; i < sizeof smalls / sizeof *smalls; ++i)
    {
        uint64_t p = smalls[i];
        if (n == p) return true;
        if (n % p == 0) return false;
    }

    /* write n-1 as d * 2^s */
    uint64_t d = n - 1;
    int s = 0;
    while ((d & 1ULL) == 0) {
        d >>= 1;
        ++s;
    }

    static const uint64_t bases[] = {2,3,5,7,11,13,17};
    for (size_t i = 0; i < sizeof bases / sizeof *bases; ++i)
    {
        uint64_t a = bases[i];
        if (a % n == 0) continue;
        uint64_t x = mod_pow(a, d, n);
        if (x == 1 || x == n - 1) continue;
        bool cont = false;
        for (int r = 1; r < s; ++r)
        {
            x = mod_mul(x, x, n);
            if (x == n - 1) { cont = true; break; }
        }
        if (!cont) return false;
    }
    return true;
}

/* Return the n-th prime ≡ 57 (mod 61).  The sequence is 1‑based. */
seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;          /* offset is 1 */

    /* cache of already found primes */
    static seq_t *cache = NULL;
    static size_t cache_size = 0;
    static size_t cache_cap  = 0;

    size_t needed = (size_t)n;
    if (cache_size < needed)
    {
        /* enlarge the cache if necessary */
        if (cache_cap < needed)
        {
            size_t new_cap = (needed > cache_cap * 2 + 16) ?
                              needed : cache_cap * 2 + 16;
            seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            if (!new_cache) abort();      /* memory failure */
            cache = new_cache;
            cache_cap = new_cap;
        }

        /* start generating primes */
        uint64_t candidate;
        if (cache_size == 0)
            candidate = 179;                /* first prime ≡ 57 (mod 61) */
        else
            candidate = (uint64_t)cache[cache_size - 1] + 61;

        while (cache_size < needed)
        {
            /* find next prime in this residue class */
            while (!is_prime_uint64(candidate))
                candidate += 61;
            cache[cache_size++] = (seq_t)candidate;
            candidate += 61;                 /* next candidate in the class */
        }
    }
    return cache[n - 1];
}
