/*
 * A142768  Primes congruent to 41 mod 59.
 * https://oeis.org/A142768
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = the n-th prime congruent to 41 (mod 59)
 * Range:     u8 n <= 1, u16 n <= 108, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 56 terms through n = 995 (2026-11-03)
 * Helpers:   is_prime_u128, is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* (offset 1) */

#include <stdlib.h>
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

static seq_t *cache = NULL;          /* array of found primes */
static size_t  cap   = 0;            /* capacity of cache array   */
static size_t  count = 0;            /* number of filled elements  */
static seq_t   next_candidate = 41;  /* first candidate to test   */

seq_t a(idx_t n)
{
    /* Return the n-th prime, 1‑based indexing. */
    /* Grow the cache until it contains at least n elements. */
    size_t needed = (size_t)n;
    if (needed > count)
    {
        if (cache == NULL)
        {
            /* initialise cache */
            cap = 16;
            cache = mem_new(seq_t, cap);
            if (!cache) abort();
            next_candidate = 41;
        }
        /* Test candidates until we have enough primes. */
        while (count < needed)
        {
            /* Check if current candidate is prime. */
            if (isPrimeGeneric(next_candidate))
            {
                /* Ensure capacity. */
                if (count == cap)
                {
                    size_t newcap = cap * 2;
                    seq_t *newptr = mem_resize(seq_t, cache, newcap);
                    if (!newptr) abort();
                    cache = newptr;
                    cap   = newcap;
                }
                cache[count++] = next_candidate;
            }
            next_candidate += 59;   /* Next number ≡ 41 (mod 59) */
        }
    }
    /* Return the cached result. */
    return cache[needed - 1];
}
