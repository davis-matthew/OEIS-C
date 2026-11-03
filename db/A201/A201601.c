/*
 * A201601  Primes of the form 6n^2 + 7.
 * https://oeis.org/A201601
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 38, u32 n <= 4308, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Validated: standard, pass, 54 terms through n = 4992 (2026-11-03)
 * Helpers:   is_prime_u128, is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Primes of the form 6*n^2 + 7 (offset 1) */
/* The function generates primes of this form by brute‑force testing
   numbers 6*k^2 + 7, caching the results for fast look‑up.  */
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>
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
    /* Offset 1: a(1) is the first prime of the form 6*n^2+7. */
    if (n == 0)            /* behaviour below offset is irrelevant */
        return 0;

    static seq_t *cache   = NULL;
    static size_t cache_cap = 0;
    static size_t cache_cnt = 0;
    static idx_t k = 0;                /* next k to test */

    /* Generate until we have at least n values cached. */
    while (cache_cnt < n) {
        seq_t val;
        /* Compute 6*k^2 + 7 in a wider integer to avoid overflow of
           intermediate products, then cast down to seq_t. */
        uint64_t tmp = 6ULL * (uint64_t)k * (uint64_t)k + 7ULL;
        val = (seq_t)tmp;
        ++k;

        /* Test primality.  The utility isPrimeGeneric works for all
           supported seq_t sizes, including big integers. */
        if (!isPrimeGeneric(val))
            continue;          /* skip composite values */

        /* Ensure the cache has room. */
        if (cache_cnt == cache_cap) {
            size_t new_cap = cache_cap ? cache_cap * 2 : 64;
            seq_t *tmp_ptr = mem_resize(seq_t, cache, new_cap);
            if (!tmp_ptr)
                abort();                   /* Out of memory. */
            cache = tmp_ptr;
            cache_cap = new_cap;
        }
        cache[cache_cnt++] = val;
    }

    return cache[n - 1];
}
