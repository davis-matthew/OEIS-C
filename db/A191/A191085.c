/*
 * A191085  Primes p that have Kronecker symbol (p|91) = -1.
 * https://oeis.org/A191085
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 25, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 70 terms through n = 933 (2026-11-03)
 * Helpers:   is_prime_u128, is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A191085:
   primes p such that the Kronecker (Jacobi) symbol (p|91) = -1. */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>

#include <limits.h>
#include "../utils/is_prime_u128.h"
#include "../utils/is_prime_u64.h"
#include "../utils/mem.h"

/* Jacobi symbol (a|n) for odd positive n <= 2^64-1.
   Returns 1, 0, or -1. */
static int jacobi_symbol(uint64_t a, uint64_t n) {
    if (n == 0) return 0;
    a %= n;
    if (a == 0) return 0;
    int res = 1;
    while (a != 0) {
        while ((a & 1) == 0) {
            a >>= 1;
            uint64_t n_mod8 = n & 7;
            if (n_mod8 == 3 || n_mod8 == 5)
                res = -res;
        }
        uint64_t tmp = a;
        a = n;
        n = tmp;
        if ((a & 3) == 3 && (n & 3) == 3)
            res = -res;
        a %= n;
    }
    return (n == 1) ? res : 0;
}

/* -----------------  Generic primality test ------------------------ */
static bool isPrimeGeneric(seq_t n)
{
    /* all seq_t are unsigned; use the smallest type that fits */
    if ((seq_t)(ULLONG_MAX) >= n) {
        return is_prime_u64((uint64_t)n);
    }
#if defined(__SIZEOF_INT128__)
    return is_prime_u128((__uint128_t)n);
#else
    /* fallback: if seq_t > 64‑bit but no 128‑bit support,
       order the test as simple trial division up to sqrt */
    seq_t limit = 0;
    for (seq_t i = 2; i*i <= n; ++i) {
        if (n % i == 0) return false;
    }
    return true;
#endif
}

/* Cache of primes satisfying the condition */
static seq_t *cache    = NULL;   /* array of primes found */
static size_t cache_len = 0;     /* current number of cached primes */
static size_t cache_cap = 0;     /* capacity of the cache array */
static uint64_t next_candidate = 2; /* next integer to test for primality */
static bool initialized = false;

/* Helper to grow the cache when needed */
static void grow_cache(size_t min_cap) {
    if (cache_cap < min_cap) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 128;
        if (new_cap < min_cap) new_cap = min_cap;
        seq_t *tmp = mem_resize(seq_t, cache, new_cap);
        if (!tmp) abort(); /* memory exhaustion */
        cache = tmp;
        cache_cap = new_cap;
    }
}

/* Main sequence function */
seq_t a(idx_t n) {
    /* Ensure cache is initialized */
    if (!initialized) {
        initialized = true;
        cache_cap   = 128;
        cache       = mem_new(seq_t, cache_cap);
        if (!cache) abort();
    }

    /* Generate primes with the desired property until we have n of them */
    while (cache_len < (size_t)n) {
        uint64_t p = next_candidate;

        /* Handle the first prime 2 specially */
        if (p > 2 && (p & 1) == 0) {
            p++; /* make odd */
        }

        /* Find next prime */
        while (!isPrimeGeneric((seq_t)p)) {
            p += 2; /* skip even numbers */
        }

        /* Check the Jacobi symbol condition */
        if (jacobi_symbol(p, 91) == -1) {
            grow_cache(cache_len + 1);
            cache[cache_len++] = (seq_t)p;
        }

        /* Set up next candidate */
        next_candidate = (p == 2) ? 3 : p + 2;
    }

    return cache[(size_t)n - 1];
}
