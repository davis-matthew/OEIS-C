/*
 * A201482  Primes of the form 5n^2 + 3.
 * https://oeis.org/A201482
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 22, u32 n <= 2310, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Validated: standard, pass, 50 terms through n = 4962 (2026-11-03)
 * Helpers:   is_prime_u128, is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implementation: primes of the form 5*n^2+3 by sequential search */

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

seq_t a(idx_t n)
{
    /* Static cache of primes found so far.  It grows until it contains at
       least n primes and then returns the (n‑1)-th element.  */
    static seq_t *cache   = NULL;   /* array of primes */
    static size_t  cap    = 0;      /* allocated capacity */
    static size_t  len    = 0;      /* number of stored primes */
    static idx_t   next_k = 0;      /* next k to test */

    /* Ensure the cache holds at least n primes. */
    while (len < (size_t)n)
    {
        /* Compute 5*k^2 + 3, using seq_t arithmetic to work with
           arbitrary‑precision builds. */
        seq_t val = (seq_t)5 * (seq_t)next_k * (seq_t)next_k + (seq_t)3;

        /* Prime test for the current value.  isPrimeGeneric handles
           both ordinary integer types and the big integer class. */
        if (isPrimeGeneric(val))
        {
            /* Resize the cache if necessary. */
            if (len == cap)
            {
                size_t newCap = (cap == 0) ? 8 : cap * 2;
                seq_t *tmp = mem_resize(seq_t, cache, newCap);
                if (!tmp) abort();          /* allocation failure unlikely */
                cache = tmp;
                cap   = newCap;
            }
            cache[len++] = val;
        }
        ++next_k;
    }

    return cache[n - 1];
}
