/*
 * A215153  Primes congruent to {1, 2, 3} mod 17.
 * https://oeis.org/A215153
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = nth prime p such that p mod 17 ∈ {1, 2, 3} (offset 1)
 * Range:     u8 n <= 13, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 63 terms through n = 981 (2026-11-03)
 * Helpers:   is_prime_u128, is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include <limits.h>
#include "../utils/is_prime_u128.h"
#include "../utils/is_prime_u64.h"
#include "../utils/mem.h"

/* Append a value to the cached array, expanding it if necessary.  `p` is a
   pointer to the array pointer. */
static void append_candidate(seq_t **p, size_t *len, size_t *cap, seq_t val)
{
    if (*len == *cap) {
        size_t newcap = (*cap == 0) ? 16 : (*cap << 1);
        seq_t *tmp = mem_resize(seq_t, *p, newcap);
        if (!tmp) {
            exit(EXIT_FAILURE);
        }
        *p = tmp;
        *cap = newcap;
    }
    (*p)[*len] = val;
    (*len)++;
}

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
    if (n == 0) return 0;           /* offset starts at 1, below is unused */

    /* static cache of sequence values   */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;
    static size_t cache_cap = 0;

    /* next integer to test for primality (starts at 2)   */
    static uint64_t next_candidate = 2;

    /* if we already have the term, return it      */
    if ((idx_t)cache_len >= n)
        return cache[n - 1];

    /* extend cache until it covers the requested index   */
    while ((idx_t)cache_len < n)
    {
        /* test next_candidate for primality
           For small numbers like those we tackle, isPrimeGeneric
           is fast enough.                                   */
        if (isPrimeGeneric((seq_t)next_candidate))
        {
            uint64_t mod = next_candidate % 17U;
            if (mod == 1U || mod == 2U || mod == 3U)
                append_candidate(&cache, &cache_len, &cache_cap,
                                 (seq_t)next_candidate);
        }
        ++next_candidate;
    }

    return cache[n - 1];
}
