/*
 * A038935  Primes p such that 57 is a square mod p.
 * https://oeis.org/A038935
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 24, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 62 terms through n = 953 (2026-11-03)
 * Helpers:   is_prime_u128, is_prime_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The sequence: primes p for which 57 is a quadratic residue modulo p
 * (including p = 2).  The function searches primes incrementally and
 * caches the result so that calls are independent of order. */

#include <stdint.h>
#include <stddef.h>
#include <assert.h>
#include <stdlib.h>
#include <stdbool.h>
#include <limits.h>
#include "../utils/is_prime_u128.h"
#include "../utils/is_prime_u64.h"

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

static seq_t* seq_realloc(seq_t* old, size_t old_cap, size_t new_cap) {
#ifdef __cplusplus
    seq_t* new_ptr = new seq_t[new_cap];
    for (size_t i = 0; i < old_cap; ++i)
        new_ptr[i] = old[i];
    delete[] old;
    return new_ptr;
#else
    return (seq_t*)realloc(old, new_cap * sizeof(seq_t));
#endif
}

seq_t a(idx_t n) {
    /* static cache so that calls in any order stay independent */
    static seq_t *cache = NULL;
    static size_t len = 0, cap = 0;
    static idx_t cand = 2;   /* next integer to test, starts at 2 */

    if (n == 0) return 0;    /* offset 1 – behaviour below offset doesn't matter */

    /* extend the cache until it contains at least n terms        */
    while (len < n) {
        /* find next prime that satisfies the condition           */
        while (1) {
            if (isPrimeGeneric((seq_t)cand)) {
                /* 2 is included automatically; for any other prime
                 * include it if 57 is a quadratic residue (Legendre=1)
                 * or if the prime divides 57 (Jacobi=0).            */
                if (cand == 2 || jacobi_symbol(57, (uint64_t)cand) != -1)
                    break;
            }
            cand++;
        }

        /* enlarge cache if necessary */
        if (len == cap) {
            size_t newcap = cap ? cap << 1 : 16;
            seq_t *tmp = seq_realloc(cache, cap, newcap); /* seq_realloc handles any seq_t type */
            assert(tmp != NULL);
            cache = tmp;
            cap = newcap;
        }

        cache[len++] = (seq_t)cand;
        cand++;                                   /* next candidate for next iteration */
    }

    return cache[n - 1];
}
