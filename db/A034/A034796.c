/*
 * A034796  a(1)=1, a(n-1) is a square mod a(n), and a(n) > a(n-1).
 * https://oeis.org/A034796
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, nice
 * Formula:   not stated
 * Range:     u8 n <= 60, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   mod_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implementation of OEIS A034796
 * a(1) = 1
 * a(n) = the smallest integer > a(n‑1) such that a(n‑1) is a quadratic residue modulo a(n)
 * (i.e. (a(n‑1))^( (a(n)–1)/2 ) ≡ 1 (mod a(n)) for all prime powers dividing a(n))
 * The function is memoised with a fixed‑size static cache. */

#include <stddef.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/mod_pow.h"   /* provides seq_t mod_pow(seq_t, uint64_t, seq_t) */

#define CACHE_CAP 200000   /* enough for n up to about 200 000 */

/* Static cache */
static seq_t cache[CACHE_CAP];
static size_t cache_size = 0;   /* number of terms currently stored */

/* Initialise the cache with a(1) */
static void init_cache(void)
{
    if (cache_size == 0) {
        cache[0] = (seq_t)1;
        cache_size = 1;
    }
}

/* Return true iff r is a quadratic residue modulo m.
 * For all prime powers p^e dividing m we apply the standard criterion:
 *   * for p = 2, e = 1: all residues allowed;
 *   * for p = 2, e = 2: residue must be 0 or 1 modulo 4;
 *   * for p = 2, e ≥ 3: residue must be 0, 1 or 4 modulo 8;
 *   * for odd p: Legendre symbol must be +1, or residue divisible by p^e. */
static bool is_quadratic_residue(seq_t r, seq_t m)
{
    /* Zero residue is always a quadratic residue. */
    if (r % m == 0)
        return true;

    /* Work with the 64‑bit value of m (acceptable because the terms up to the
     * required range all fit into 64 bits). */
    uint64_t mm = (uint64_t)m;
    uint64_t r_mod_m = (uint64_t)(r % (seq_t)mm);  /* 0 ≤ r_mod_m < mm */

    /* Factorisation of mm into primes and exponents. */
    uint64_t primes[32];
    int exps[32];
    int cnt = 0;

    if (mm == 0)
        return false;

    if ((mm & 1ULL) == 0) {
        int e = 0;
        while ((mm & 1ULL) == 0) { mm >>= 1; ++e; }
        primes[cnt]   = 2ULL;
        exps[cnt]     = e;
        ++cnt;
    }
    for (uint64_t p = 3ULL; p * p <= mm; p += 2ULL) {
        if (mm % p == 0) {
            int e = 0;
            while (mm % p == 0) { mm /= p; ++e; }
            primes[cnt]   = p;
            exps[cnt]     = e;
            ++cnt;
        }
    }
    if (mm > 1) {
        primes[cnt]   = mm;
        exps[cnt]     = 1;
        ++cnt;
    }

    /* Check each prime power. */
    for (int i = 0; i < cnt; ++i) {
        uint64_t p = primes[i];
        int e      = exps[i];

        if (p == 2ULL) {
            if (e == 1)
                continue; /* all residues acceptable */

            if (e == 2) {
                /* residues 0 or 1 modulo 4 */
                if ((r_mod_m & 3ULL) > 1)
                    return false;
                continue;
            }

            /* e >= 3: residues 0, 1, 4 modulo 8 are allowed */
            uint64_t rem8 = r_mod_m & 7ULL;
            if (rem8 != 0ULL && rem8 != 1ULL && rem8 != 4ULL)
                return false;
        } else {
            /* odd prime */
            uint64_t r0 = r_mod_m % p;
            if (r0 == 0ULL) {
                /* must be divisible by p^e */
                uint64_t pe = 1ULL;
                for (int j = 0; j < e; ++j) pe *= p;
                if (r_mod_m % pe != 0ULL)
                    return false;
                continue;
            }
            /* Legendre symbol via Euler's criterion */
            seq_t leg = mod_pow((seq_t)r0, (p - 1ULL) / 2ULL, (seq_t)p);
            if (leg == (seq_t)(p - 1ULL))
                return false;   /* ≡ -1 (mod p) */
        }
    }
    return true;
}

/* Compute a(n) */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;        /* undefined for offset 0, return 0 */

    /* Ensure the cache is initialised */
    init_cache();

    /* Extend the cache up to n if needed */
    while (cache_size < n) {
        seq_t prev = cache[cache_size - 1];
        seq_t m    = prev + (seq_t)1;
        for (;; ++m) {
            if (is_quadratic_residue(prev, m))
                break;
        }
        /* Store the new term */
        cache[cache_size] = m;
        ++cache_size;
    }
    return cache[n - 1];
}
