/*
 * A049423  Primes of the form k^2 + 3.
 * https://oeis.org/A049423
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 35, u32 n <= 3708, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 55 terms through n = 8974 (2026-11-03)
 * Helpers:   is_prime_u128, is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This function returns the n‑th prime of the form k² + 3.
   For n = 1 the result is 3, for n = 2 it is 7, etc.
   The implementation lazily generates candidates, tests primality
   with isPrimeGeneric, and caches found primes so that calls
   in any order remain fast. */

#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <limits.h>
#include "../utils/is_prime_u128.h"
#include "../utils/is_prime_u64.h"
#include "../utils/mem.h"

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

static seq_t *alloc_seq_array(size_t n)
{
    return mem_new(seq_t, n);
}

static void free_seq_array(seq_t *p)
{
    mem_free(p);
}

seq_t a(idx_t n)
{
    /* Lazy static cache of found primes */
    static seq_t *cache      = NULL; /* array of primes */
    static size_t cache_count = 0;    /* number of elements stored */
    static size_t cache_cap   = 0;    /* allocated capacity of cache */
    static idx_t last_k       = 0;    /* next k to test (starting at 0) */

    if (n == 0) {
        return (seq_t)0;                /* undefined for n < 1 */
    }

    /* Grow cache until it contains at least n terms. */
    while (cache_count < n) {
        seq_t candidate = (seq_t)last_k * (seq_t)last_k + (seq_t)3;

        if (isPrimeGeneric(candidate)) {
            /* Ensure there is room for another element. */
            if (cache_count >= cache_cap) {
                size_t new_cap = cache_cap ? cache_cap * 2U : 128U;
                seq_t *tmp = alloc_seq_array(new_cap);
                if (!tmp) {
                    /* Allocation failed – abort. */
                    exit(EXIT_FAILURE);
                }
                /* Copy existing elements to the new array. */
                for (size_t i = 0; i < cache_count; ++i) {
                    tmp[i] = cache[i];
                }
                /* Free old array and update pointers/capacity. */
                if (cache) {
                    free_seq_array(cache);
                }
                cache = tmp;
                cache_cap = new_cap;
            }
            /* Store the new prime. */
            cache[cache_count++] = candidate;
        }
        ++last_k;
    }

    return cache[n - 1];
}
