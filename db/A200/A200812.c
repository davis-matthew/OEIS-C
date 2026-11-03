/*
 * A200812  Primes of the form 5*n^3-6.
 * https://oeis.org/A200812
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 3, u32 n <= 69, u64 n <= 1100, u128 n <= 1100,
 *            big any n
 * Validated: standard, pass, 41 terms through n = 1099 (2026-11-03)
 * Helpers:   is_prime_u128, is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the nth prime of the form 5*n^3 - 6 (OEIS A200812). */
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

/* Returns a(n) for n >= 1; undefined for n == 0 (behaviour below the
   offset is irrelevant). */
seq_t a(idx_t n)
{
    if (n == 0) return 0;            /* caller may ignore base case */

    /* --- static cache for previously found primes --- */
    static idx_t   next_k  = 1;      /* next k to test */
    static seq_t * cache   = NULL;   /* array of found primes */
    static size_t  cap     = 0;      /* allocated capacity */
    static size_t  len     = 0;      /* current number of primes stored */

    /* Grow the cache until we have at least n primes. */
    while (len < n) {
        seq_t k = (seq_t)next_k++;           /* current k value */

        /* Compute 5*k^3 - 6 safely in seq_t. */
        seq_t k2 = k * k;
        seq_t k3 = k2 * k;
        seq_t val = 5 * k3;
        if (val <= 6) continue;      /* skip (val - 6) would underflow */
        val -= 6;                    /* val = 5*k^3 - 6 */

        /* Test primality.  isPrimeGeneric works for all seq_t sizes. */
        if (isPrimeGeneric(val)) {
            /* If the cache is full, grow it (double the size). */
            if (len == cap) {
                size_t new_cap = cap ? cap * 2 : 8;
                /* cast is needed for C++ compatibility when seq_t is a class. */
                seq_t *tmp = mem_resize(seq_t, cache, new_cap);
                if (!tmp) abort();
                cache = tmp;
                cap = new_cap;
            }
            cache[len++] = val;
        }
    }
    return cache[n - 1];
}
