/*
 * A086085  Primes p such that p + floor(sqrt(p)) is prime.
 * https://oeis.org/A086085
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 11, u16 n <= 652, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   int_sqrt, is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A Function to generate OEIS A086085: primes p such that p + floor(sqrt(p)) is prime
 * The sequence is indexed from 1; a(1) = 2.
 * This implementation lazily builds the sequence on demand and caches the
 * results.  It works for any seq_t and idx_t type that satisfy the build's
 * conventions, and it uses only the helper functions provided by the
 * environment.
 */

#include <stddef.h>      /* size_t */
#include <stdint.h>      /* uint64_t */
#include <stdbool.h>     /* bool */

/* Helpers likely provided by the build system.  Include their headers. */
#include "../utils/is_prime.h"       /* bool is_prime(idx_t x); */
#include "../utils/int_sqrt.h"      /* idx_t int_sqrt(idx_t n); */

/* Return the n-th term of A086085 (n >= 1). */
seq_t a(idx_t n)
{
    /* 0‑index safety: if called with n == 0, return 0.  OEIS offset is 1. */
    if (n == 0)
        return (seq_t)0;

    /* Static cache of found primes.  The array is grown as needed. */
    static seq_t  *cache      = NULL;  /* array of cached terms   */
    static size_t cache_sz   = 0;     /* number of cached terms  */
    static size_t cache_cap  = 0;     /* current array capacity  */
    static idx_t  last_checked = 0;   /* last candidate examined */

    /* Allocate the cache on first use. */
    if (!cache) {
        cache_cap = 128;                      /* initial capacity */
        cache     = (seq_t *)malloc(cache_cap * sizeof(seq_t));
    }

    /* Extend the cache until we have at least n terms. */
    while (cache_sz < (size_t)n) {
        /* Next candidate to test. */
        idx_t candidate = last_checked + 1;
        last_checked = candidate;

        /* Must be a prime. */
        if (!is_prime(candidate))
            continue;

        /* Compute candidate + floor(sqrt(candidate)). */
        idx_t sqrt_floor = int_sqrt(candidate);
        idx_t sum = candidate + sqrt_floor;

        /* The sum must also be prime. */
        if (!is_prime(sum))
            continue;

        /* Store the valid prime. */
        if (cache_sz == cache_cap) {
            cache_cap <<= 1;                                      /* double capacity */
            cache = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
        }
        cache[cache_sz++] = (seq_t)candidate;
    }

    /* Return the requested term (1‑based index). */
    return cache[n - 1];
}
