/*
 * A214588  Primes p such that p mod 16 < 8.
 * https://oeis.org/A214588
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 30, u16 n <= 3284, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 72 terms through n = 9127 (2026-11-03)
 * Helpers:   is_prime_u128, is_prime_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This function returns the n-th prime p such that p mod 16 < 8
   (equivalently p XOR 8 = p + 8).  It builds a static cache of primes
   satisfying the condition and expands it lazily as needed. */

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
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

seq_t a(idx_t n)
{
    /* static cache of primes that satisfy p mod 16 < 8 */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;   /* current capacity of cache */
    static size_t cache_size = 0;  /* number of primes stored */

    /* next integer to test for primality */
    static idx_t next_candidate = 2;

    /* grow the cache until it contains at least n primes */
    while (cache_size < n)
    {
        seq_t cand = (seq_t)next_candidate;
        bool is_p;

        /* trivial checks for candidate */
        if (next_candidate == 2) {
            is_p = true;              /* 2 is prime */
        } else if ((next_candidate & 1ULL) == 0) {
            is_p = false;             /* even numbers > 2 are composite */
        } else {
            is_p = isPrimeGeneric(cand);
        }

        /* store if it is prime and satisfies the condition */
        if (is_p && ((next_candidate & 0xFULL) < 8ULL))
        {
            if (cache_size == cache_cap)   /* grow cache if needed */
            {
                size_t new_cap = (cache_cap == 0) ? 16 : cache_cap * 2;
                cache = seq_realloc(cache, cache_cap, new_cap);
                cache_cap = new_cap;
            }
            cache[cache_size++] = cand;
        }

        ++next_candidate;   /* advance to the next integer */
    }

    return cache[n - 1];
}
