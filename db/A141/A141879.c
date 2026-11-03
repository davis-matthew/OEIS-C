/*
 * A141879  Primes congruent to 14 mod 19.
 * https://oeis.org/A141879
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) is the n‑th prime congruent to 14 (mod 19)
 * Range:     u8 n <= 3, u16 n <= 353, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 60 terms through n = 952 (2026-11-03)
 * Helpers:   is_prime_u128, is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The implementation searches primes in increasing order,
   caching the values so that subsequent calls are fast.
   Its cache grows with mem_resize, valid in C and C++. */

#include <stdlib.h>
#include <string.h>
#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
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

/* Reallocate an array of seq_t. */
static seq_t *realloc_arr(seq_t *oldptr, size_t oldbytes, size_t newbytes)
{
    (void)oldbytes;
    return mem_resize(seq_t, oldptr, newbytes / sizeof(seq_t));
}

/* Allocate an array of seq_t. */
static seq_t *alloc_arr(size_t bytes)
{
    return mem_new(seq_t, bytes / sizeof(seq_t));
}

/* Return the n-th prime ≡ 14 (mod 19).  n ≥ 1. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;                /* unused, keeps interface clean */

    /* static state for the cache of found primes */
    static seq_t *cache   = NULL;        /* array of found primes      */
    static idx_t  cap     = 0;           /* current allocated capacity */
    static idx_t  used    = 0;           /* number of primes stored    */
    static seq_t  next    = 71;          /* first prime ≡ 14 (mod 19)  */

    /* initialise cache on first call */
    if (cap == 0) {
        cap = 16;
        cache = (seq_t*)alloc_arr(cap * sizeof(seq_t));
        if (!cache) return 0;            /* allocation failed */
    }

    /* generate primes until we have at least n entries */
    while (used < n) {
        /* grow the cache if full */
        if (used == cap) {
            idx_t newcap = cap * 2;
            cache = (seq_t*)realloc_arr(cache, cap * sizeof(seq_t),
                                         newcap * sizeof(seq_t));
            if (!cache) return 0;        /* allocation failed */
            cap = newcap;
        }

        /* test current candidate */
        if (isPrimeGeneric(next))
            cache[used++] = next;

        /* advance to the next number ≡ 14 (mod 19) */
        next += 19;
    }

    return cache[n - 1];                  /* 1‑based index */
}
