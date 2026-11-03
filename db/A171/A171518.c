/*
 * A171518  Primes p such that 3*p-+8 are primes.
 * https://oeis.org/A171518
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 315, u32 n <= 3000, u64 n <= 3000,
 *            u128 n <= 3000, big any n
 * Validated: standard, pass, 64 terms through n = 2920 (2026-11-03)
 * Helpers:   is_prime, is_prime_u128, is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This function returns the n-th prime p for which both 3*p-8
   and 3*p+8 are also primes.  It builds the sequence on demand
   and memoises the results.  The implementation works for any
   seq_t as long as the required utility functions are available. */
#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <limits.h>
#include "../utils/is_prime.h"
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

static seq_t *cache = NULL;          /* cached primes */
static idx_t cached_count = 0;       /* number of stored terms */
static size_t cache_cap = 0;         /* current capacity of cache   */
static uint64_t last_checked = 1;    /* last integer examined      */

/* Ensure the cache can hold at least 'need' elements.  */
static void ensure_cache_capacity(idx_t need)
{
    if (cache_cap >= (size_t)need) return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 8;
    while (new_cap < (size_t)need) new_cap *= 2;
    seq_t *new_buf = mem_resize(seq_t, cache, new_cap);
    cache = new_buf;
    cache_cap = new_cap;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* defined only for n>=1 */

    /* Extend the cache until it contains at least n terms. */
    while (cached_count < n) {
        /* Find the next prime candidate.  */
        uint64_t p = last_checked + 1;
        while (!is_prime(p)) ++p;
        last_checked = p;

        /* Check the property 3*p-8 and 3*p+8 are primes.  */
        seq_t sp = (seq_t)p;
        seq_t left  = 3 * sp - (seq_t)8;
        seq_t right = 3 * sp + (seq_t)8;
        if (isPrimeGeneric(left) && isPrimeGeneric(right)) {
            /* Store the qualifying prime. */
            ensure_cache_capacity(cached_count + 1);
            cache[cached_count++] = sp;
        }
    }
    return cache[n - 1];
}
