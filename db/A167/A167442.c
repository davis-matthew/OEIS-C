/*
 * A167442  Prime numbers ending in the prime number 11.
 * https://oeis.org/A167442
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 165, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 57 terms through n = 927 (2026-11-03)
 * Helpers:   is_prime_u128, is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Procedure: a(n) = the n-th prime of the form 100*k + 11 (k ≥ 0). */
/* A sequential search is used; results are cached to allow arbitrary
   order of calls. */

#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
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
    if (n == 0)
        return 0;  /* undefined, but harmless */

    /* Cache of found primes: cache[i] holds the (i+1)‑th prime of the form. */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;   /* allocated length of cache */
    static size_t cache_len = 0;   /* number of primes actually stored */
    static idx_t next_i = 0;       /* index of the next candidate 100*k+11 */

    while (cache_len < (size_t)n)
    {
        /* Candidate prime 11 + 100*next_i. */
        seq_t cand = 11 + (seq_t)(next_i * 100ULL);

        if (isPrimeGeneric(cand))
        {
            /* Grow the cache if required. */
            if (cache_len + 1 > cache_cap)
            {
                size_t new_cap = cache_cap ? cache_cap * 2 : 16;
                void *p = mem_resize(seq_t, cache, new_cap);
                if (!p) abort();
                cache = (seq_t *)p;
                cache_cap = new_cap;
            }
            cache[cache_len++] = cand;
        }
        ++next_i;
    }

    return cache[n - 1];
}
