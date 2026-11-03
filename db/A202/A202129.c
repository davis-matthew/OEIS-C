/*
 * A202129  Numbers n such that 90n + 71 is prime.
 * https://oeis.org/A202129
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 61, u32 n <= 61, u64 n <= 61, u128 n <= 61,
 *            big any n
 * Validated: partial, pass, 61 terms through n = 61 (2026-11-03)
 * Helpers:   is_prime_u128, is_prime_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * OEIS: A202129
 * Description: Numbers n for which 90n + 71 is prime.
 * Sequence offset: 1  (a(1) = 0)
 * This code lazily computes the terms using a simple sieve-like scan
 * and memoises them so that any order of calls works.
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
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

/* Grow a dynamic array when needed.  The pointer is passed as (void **). */
static void ensure_capacity(size_t needed, size_t *cap, void **ptr, size_t elem_size)
{
    if (needed <= *cap) return;
    size_t new_cap = (*cap == 0) ? 4 : (*cap << 1);
    while (new_cap < needed) new_cap <<= 1;
    void *tmp = realloc(*ptr, new_cap * elem_size);
    if (!tmp) abort();               /* on OOM we abort as required */
    *ptr = tmp;
    *cap = new_cap;
}

seq_t a(idx_t n)
{
    /* static cache for memoisation */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;      /* allocated capacity */
    static size_t cache_len = 0;      /* number of terms stored */
    static idx_t next_candidate = 0;  /* next n to test */

    /* if we already have the term return it */
    if (cache_len >= n)
        return cache[n - 1];

    /* grow cache if required */
    if (cache_cap < (size_t)n)
        ensure_capacity((size_t)n, &cache_cap, (void **)&cache, sizeof(seq_t));

    /* scan candidates until enough terms have been gathered */
    while (cache_len < n) {
        /* compute 90*candidate + 71 as seq_t to avoid overflow in idx_t */
        seq_t val = (seq_t)90 * (seq_t)next_candidate + 71;
        if (isPrimeGeneric(val)) {
            cache[cache_len++] = next_candidate;
        }
        ++next_candidate;
    }

    return cache[n - 1];
}
