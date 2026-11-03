/*
 * A178066  Primes p of the form x^2+59, such that p+2 is also prime.
 * https://oeis.org/A178066
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the n-th prime of the form 100*k^2+59 whose successor p+2
 *            is also prime
 * Range:     u8 n <= 1, u16 n <= 1, u32 n <= 46, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 41 terms through n = 956 (2026-11-03)
 * Helpers:   is_prime_u128, is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The function searches sequentially for such primes, storing results in a static cache so that calls
   with arbitrary n are served efficiently and in any order. */

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

seq_t a(idx_t n)
{
    /* Lazy, memoised generation of the sequence. */
    static seq_t *cache   = NULL;   /* holds the found primes */
    static idx_t size     = 0;      /* number of stored terms */
    static idx_t capacity = 0;      /* current allocation size */
    static idx_t next_k   = 0;      /* k to start next search from */

    if (n < 1) return 0; /* undefined for n = 0 */

    /* Grow the cache when needed.  realloc is used so that the allocation
       overhead stays low and copying of old data is handled by the library. */
    if (size < n && capacity < n)
    {
        size_t new_cap = capacity == 0 ? 128 : capacity * 2;
        if (new_cap < n) new_cap = n;
        seq_t *tmp = mem_resize(seq_t, cache, new_cap);
        if (!tmp) return 0; /* allocation failure */
        cache    = tmp;
        capacity = new_cap;
    }

    /* Generate primes until we have at least n terms. */
    while (size < n)
    {
        /* Compute candidate p = 100*k^2 + 59 */
        seq_t k = (seq_t)next_k;
        seq_t k2 = k * k;                     /* k^2          */
        seq_t p  = k2 * 100 + 59;             /* 100*k^2 + 59 */

        /* Handle overflow for 64‑bit seq_t builds.  If the value would overflow
           beyond the maximum representable value of seq_t, simply stop the loop
           – further values cannot be represented correctly. */
#ifdef UINT64_MAX
        if (sizeof(seq_t) == sizeof(uint64_t))
        {
            /* In 64‑bit builds test against overflow condition. */
            if (k2 > ((uint64_t)(~0ULL) - 59) / 100ULL)
            {
                /* Cannot represent any larger p; premature exit.*/
                return 0;
            }
        }
#endif
        /* Check primality of p and its twin p+2.  Use the generic routine so
           it works for all supported seq_t types (small or arbitrary). */
        if (isPrimeGeneric(p) && isPrimeGeneric(p + 2))
        {
            cache[size++] = p;
        }
        next_k++;
    }

    return cache[n - 1];
}
