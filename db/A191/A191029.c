/*
 * A191029  Primes p with Jacobi symbol (p|39) = 1.
 * https://oeis.org/A191029
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = the n-th prime p with Jacobi(p,39) = 1 (p ≠ 3,13)
 * Range:     u8 n <= 25, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   mul_mod64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation builds the sequence by generating primes
   one by one, testing the Jacobi condition by simple
   modular arithmetic modulo 3 and 13.  A deterministic
   Miller‑Rabin test is used for 64‑bit primes.  The
   results are cached so that calls in arbitrary order are fast. */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/mul_mod64.h"

/* Forward declarations of helpers (required only for the pragma
   environment but not used directly).  They are not linked,
   therefore not included. */

/* ----- Small helpers --------------------------------------------------- */

/* 64‑bit modular multiplication: (a*b) % mod  (no overflow) */

/* 64‑bit modular exponentiation: (base^exp) % mod */
static uint64_t pow_mod64(uint64_t base, uint64_t exp, uint64_t mod)
{
    uint64_t result = 1;
    while (exp) {
        if (exp & 1) result = mul_mod64(result, base, mod);
        base = mul_mod64(base, base, mod);
        exp >>= 1;
    }
    return result;
}

/* Deterministic Miller‑Rabin for 64‑bit integers.
   Works for all n < 2^64. */
static bool is_prime_uint64(uint64_t n)
{
    if (n < 2) return false;
    static const uint64_t small_primes[] = {
        2, 3, 5, 7, 11, 13, 17, 19, 23, 29,
        31, 37
    };
    for (size_t i = 0; i < sizeof(small_primes) / sizeof(small_primes[0]); ++i) {
        uint64_t p = small_primes[i];
        if (n == p) return true;
        if (n % p == 0) return false;
    }

    /* write n‑1 = d·2^s with d odd */
    uint64_t d = n - 1, s = 0;
    while ((d & 1) == 0) {
        d >>= 1;
        ++s;
    }

    /* bases that cover all 64‑bit integers */
    static const uint64_t bases[] = {2, 325, 9375, 28178, 450775, 9780504, 1795265022};
    for (size_t i = 0; i < sizeof(bases) / sizeof(bases[0]); ++i) {
        uint64_t a = bases[i] % n;
        if (a == 0) continue;
        uint64_t x = pow_mod64(a, d, n);
        if (x == 1 || x == n - 1) continue;
        bool cont = false;
        for (uint64_t r = 1; r < s; ++r) {
            x = mul_mod64(x, x, n);
            if (x == n - 1) { cont = true; break; }
        }
        if (!cont) return false;
    }
    return true;
}

/* Jacobi condition for p relative to 39 = 3 * 13.
   Returns true if Jacobi(p,39) == 1 and p does not divide 39. */
static bool good_prime(uint64_t p)
{
    if (p % 3 == 0 || p % 13 == 0) return false;

    /* Legendre symbol (p|3): 1 if p ≡ 1 mod 3, -1 if p ≡ 2 mod 3 */
    int leg3 = (p % 3 == 1) ? 1 : -1;

    /* Legendre symbol (p|13): 1 if p mod 13 is a quadratic residue mod 13 */
    uint64_t m13 = p % 13;
    bool sq13 = (m13 == 1 || m13 == 3 || m13 == 4 || m13 == 9 ||
                 m13 == 10 || m13 == 12);
    int leg13 = sq13 ? 1 : -1;

    return leg3 == leg13;
}

/* ----- Cache and main routine ----------------------------------------- */

static seq_t *cache   = NULL;   /* cached sequence values */
static size_t cache_len = 0;    /* number of values cached */
static size_t cache_cap = 0;    /* allocated capacity */
static uint64_t next_candidate = 2;   /* next integer to test for primality */

/* Ensure the cache can hold 'needed' elements */
static void ensure_cache_cap(size_t needed)
{
    if (cache_cap >= needed) return;
    size_t newcap = cache_cap ? cache_cap * 2 : 128;
    while (newcap < needed) newcap <<= 1;
    seq_t *newptr = (seq_t *)malloc(newcap * sizeof(seq_t));
    if (!newptr) exit(EXIT_FAILURE);
    if (cache) {
        memcpy(newptr, cache, cache_len * sizeof(seq_t));
        free(cache);
    }
    cache = newptr;
    cache_cap = newcap;
}

/* Main sequence function required by the OEIS harness */
seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* undefined for n = 0, return 0 as safe value */

    /* Generate terms until we have at least n of them */
    while ((idx_t)cache_len < n) {
        uint64_t p;
        /* Find the next prime candidate */
        if (next_candidate == 2) {
            p = 2;
            next_candidate = 3;
        } else {
            /* Skip even numbers */
            if ((next_candidate & 1) == 0) ++next_candidate;
            do {
                if (!is_prime_uint64(next_candidate)) {
                    next_candidate += 2;
                    continue;
                }
                p = next_candidate;
                next_candidate += 2;
                break;
            } while (true);
        }

        /* If it satisfies the Jacobi condition, store it */
        if (good_prime(p)) {
            ensure_cache_cap(cache_len + 1);
            cache[cache_len++] = (seq_t)p;
        }
    }

    return cache[n - 1];
}
