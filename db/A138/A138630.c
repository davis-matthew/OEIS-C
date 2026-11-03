/*
 * A138630  Nonnegative integers k such that 17*k+7 is prime.
 * https://oeis.org/A138630
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 533, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 75 terms through n = 8727 (2026-11-03)
 * Helpers:   is_prime_u128, is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Nonnegative integers k such that 17*k+7 is prime.
   a(n) returns the n-th such k (1‑based). */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
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

static seq_t *cache = NULL;        /* array holding found k values */
static idx_t cache_len = 0;        /* number of values currently stored */
static idx_t cache_cap = 0;        /* current capacity of cache */
static idx_t next_k = 0;           /* next k to test for primality */

/* Expand cache storage to at least `need` elements. */
static void ensure_capacity(idx_t need)
{
    if (cache_len >= need)
        return;
    if (cache_cap == 0) {
        cache_cap = 8;
        cache = mem_new(seq_t, cache_cap);
    } else {
        while (cache_cap < need)
            cache_cap <<= 1;
        cache = mem_resize(seq_t, cache, cache_cap);
    }
    /* now fill until we have `need` values */
    while (cache_len < need) {
        /* compute 17*next_k + 7 using a wide type to avoid overflow of the
           multiplication for a given seq_t.  We only need the result modulo
           seq_t for the primality test, which is safe because isPrimeGeneric
           works on seq_t. */
        seq_t cand = (seq_t)(( (seq_t)next_k * (seq_t)17) + (seq_t)7);
        if (isPrimeGeneric(cand))
            cache[cache_len++] = (seq_t)next_k;
        ++next_k;
    }
}

/* Return the n-th element of the sequence (n ≥ 1). */
seq_t a(idx_t n)
{
    if (n == 0)   /* undefined for offset < 1, but safe guard */
        return (seq_t)0;
    ensure_capacity(n);
    return cache[n - 1];
}
