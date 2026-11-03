/*
 * A030431  Primes of form 10n+3.
 * https://oeis.org/A030431
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 14, u16 n <= 1646, u32 n <= 100000, u64 n <= 100000,
 *            u128 n <= 100000, big any n
 * Validated: standard, pass, 67 terms through n = 9874 (2026-11-03)
 * Helpers:   is_prime_u128, is_prime_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A030431: primes of the form 10n+3 (n ≥ 1).  This implementation searches
 * candidates of the form 10k+3 and tests them for primality. */

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

static seq_t *prime_cache = NULL;   /* array of primes ≡ 3 (mod 10) */
static size_t  cache_size = 0;      /* number of stored primes          */
static size_t  cache_cap   = 0;      /* current capacity of the array    */
static uint64_t last_candidate = 3;  /* last candidate tested (3 initially) */

/* Ensure the cache can hold 'need' elements. */
static void ensure_capacity(size_t need)
{
    if (cache_cap < need) {
        cache_cap = (cache_cap == 0) ? 8 : cache_cap * 2;
        if (cache_cap < need) cache_cap = need;
        prime_cache = seq_realloc(prime_cache, cache_size, cache_cap);
    }
}

/* Return the n-th term of the sequence (1‑based index). */
seq_t a(idx_t n)
{
    if (n == 0) return 0;                          /* undefined, safe guard */

    /* If the cache already has the desired term, return it. */
    if ((idx_t)cache_size >= n)
        return prime_cache[n - 1];

    /* Initialize the cache with the first prime 3, if not already done. */
    if (cache_size == 0) {
        ensure_capacity(1);
        prime_cache[0] = (seq_t)3;
        cache_size = 1;
        last_candidate = 3;
    }

    /* Generate further primes until we reach the desired index. */
    while ((idx_t)cache_size < n) {
        last_candidate += 10;                     /* next candidate of form 10k+3 */
        if (isPrimeGeneric((seq_t)last_candidate)) {
            ensure_capacity(cache_size + 1);
            prime_cache[cache_size++] = (seq_t)last_candidate;
        }
    }

    return prime_cache[n - 1];
}
