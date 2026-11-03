/*
 * A202114  Numbers k such that 90*k + 53 is prime.
 * https://oeis.org/A202114
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 62, u32 n <= 62, u64 n <= 62, u128 n <= 62,
 *            big any n
 * Validated: partial, pass, 62 terms through n = 62 (2026-11-03)
 * Helpers:   is_prime_u128, is_prime_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) = the nth integer k such that 90*k + 53 is prime  (offset 1). */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
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

static idx_t *cache = NULL;      /* cached k-values */
static size_t cache_size = 0;    /* number of cached terms */
static size_t cache_cap   = 0;    /* current allocation */

/* enlarge the cache so it can hold at least 'needed' elements */
static void grow_cache(size_t needed)
{
    if (needed <= cache_cap)
        return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 64;
    while (new_cap < needed)
        new_cap <<= 1;
    idx_t *p = (idx_t *)realloc(cache, new_cap * sizeof(idx_t));
    if (!p)
        exit(EXIT_FAILURE);   /* unrecoverable allocation failure */
    cache = p;
    cache_cap = new_cap;
}

seq_t a(idx_t n)
{
    if (n == 0)                /* offset is 1, undefined for 0 */
        return 0;

    /* if the requested term is already cached, return it */
    if (cache_size >= (size_t)n)
        return (seq_t)cache[n - 1];

    /* start searching from the next candidate after the last cached value */
    idx_t k = cache_size ? cache[cache_size - 1] + 1 : 0;

    /* generate terms until we reach the n-th one */
    while (cache_size < (size_t)n)
    {
        /* compute 90*k + 53; use seq_t for the result to allow big integers */
        seq_t val = (seq_t)(90ULL * (uint64_t)k + 53ULL);

        if (isPrimeGeneric(val))
        {
            grow_cache(cache_size + 1);
            cache[cache_size] = k;
            cache_size++;
        }

        k++;
    }
    return (seq_t)cache[n - 1];
}
