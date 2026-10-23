/*
 * A191058  Primes that are squares mod 97.
 * https://oeis.org/A191058
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = the nth prime p (p ≠ 97) such that p is a quadratic
 *            residue modulo 97
 * Range:     u8 n <= 24, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* (equivalently, Legendre symbol (p|97) = 1) */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>
#include "../utils/is_prime.h"      /* provides bool is_prime(idx_t x) */

static bool residues[97];
static bool residues_initialized = false;

/* modular exponentiation for 64‑bit arguments */
static uint64_t mod_pow_u64(uint64_t base, uint64_t exp, uint64_t mod)
{
    uint64_t result = 1;
    base %= mod;
    while (exp) {
        if (exp & 1)
            result = (result * base) % mod;
        base = (base * base) % mod;
        exp >>= 1;
    }
    return result;
}

/* pre‑compute the non‑zero quadratic residues modulo 97 */
static void init_residues(void)
{
    if (residues_initialized)
        return;
    for (uint64_t r = 1; r < 97; ++r)
        residues[r] = (mod_pow_u64(r, 48, 97) == 1);   /* (97-1)/2 = 48 */
    residues_initialized = true;
}

/* dynamic array of cached sequence values (stored as uint64_t) */
static uint64_t *cache_values = NULL;   /* primes satisfying the property */
static size_t cache_cap   = 0;
static size_t cache_size  = 0;

/* next integer to test for primality */
static uint64_t next_candidate = 2;

/* ensure that cache_values contains at least n valid entries */
static void ensure_cache(idx_t n)
{
    init_residues();

    while (cache_size < (size_t)n) {
        uint64_t cand = next_candidate;
        ++next_candidate;               /* advance for next call */

        /* skip even numbers > 2 */
        if (cand > 2 && (cand & 1) == 0)
            continue;

        /* primality test using the supplied utility */
        if (!is_prime((idx_t)cand))
            continue;

        /* exclude 97 itself */
        if (cand % 97 == 0)
            continue;

        /* check quadratic residue condition */
        uint64_t r = cand % 97;
        if (!residues[r])
            continue;

        /* store the prime in the cache */
        if (cache_size == cache_cap) {
            size_t new_cap = cache_cap ? cache_cap * 2 : 16;
            uint64_t *tmp = (uint64_t *)realloc(cache_values, new_cap * sizeof(*tmp));
            if (!tmp)
                abort();  /* allocation failure */
            cache_values = tmp;
            cache_cap = new_cap;
        }
        cache_values[cache_size++] = cand;
    }
}

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                 /* undefined for offset 1, harmless */
    ensure_cache(n);
    return (seq_t)cache_values[n - 1];
}
