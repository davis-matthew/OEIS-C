/*
 * A142315  Primes congruent to 7 mod 45.
 * https://oeis.org/A142315
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 274, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 61 terms through n = 992 (2026-11-03)
 * Helpers:   is_prime_u128, is_prime_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation: generate primes congruent to 7 (mod 90) */
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
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

/* Static cache for memoisation */
static seq_t *cache = NULL;
static size_t cache_len = 0;   /* number of entries currently stored   */
static size_t cache_cap = 0;   /* allocated capacity                   */
static seq_t next_candidate = (seq_t)7;   /* next number to test, 7 mod 90 */

seq_t a(idx_t n)
{
    if (n == 0)          /* out‑of‑range, but never requested */
        return (seq_t)0;

    /* Grow cache if necessary */
    if (cache_len < (size_t)n) {
        size_t required = (size_t)n;
        if (required > cache_cap) {
            size_t new_cap = cache_cap ? cache_cap * 2 : 16;
            while (new_cap < required) new_cap *= 2;
            cache = seq_realloc(cache, cache_cap, new_cap);
            cache_cap = new_cap;
        }

        /* Generate primes until we have n entries in the cache */
        while (cache_len < required) {
            if (isPrimeGeneric(next_candidate)) {
                cache[cache_len++] = next_candidate;
            }
            next_candidate += (seq_t)90;
        }
    }

    return cache[n - 1];
}
