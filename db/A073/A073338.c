/*
 * A073338  Positive values of n for which 4n^2-10n+7 is prime.
 * https://oeis.org/A073338
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 37, u32 n <= 5087, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 76 terms through n = 9842 (2026-11-03)
 * Helpers:   is_prime_u128, is_prime_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A073338: n such that 4*n^2-10*n+7 is prime. */

#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
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

static seq_t* seq_realloc(seq_t* old, size_t old_cap, size_t new_cap) {
#ifdef __cplusplus
    seq_t* new_ptr = new seq_t[new_cap];
    for (size_t i = 0; i < old_cap; ++i)
        new_ptr[i] = old[i];
    delete[] old;
    return new_ptr;
#else
    return (seq_t*)realloc(old, new_cap * sizeof(seq_t));
#endif
}

/* Static cache for memoization of the sequence. */
static seq_t *cache = NULL;
static size_t cache_len = 0;   /* number of terms stored in cache */
static size_t cache_cap = 0;   /* allocated capacity of cache */
static idx_t next_k = 1;       /* next integer to test */

/* Grow the cache capacity using seq_realloc. */
static void grow_cache(void)
{
    size_t new_cap = (cache_cap == 0) ? 16 : cache_cap * 2;
    seq_t *tmp = seq_realloc(cache, cache_cap, new_cap);
    if (!tmp) abort();                /* unrecoverable memory error */
    cache = tmp;
    cache_cap = new_cap;
}

/* Return the nth term (1‑based) of the OEIS sequence A073338. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* undefined for n=0, guard */

    while (cache_len < (size_t)n) {
        seq_t kk = (seq_t)next_k;
        seq_t val = 4 * kk * kk - 10 * kk + 7;
        if (isPrimeGeneric(val)) {
            if (cache_len == cache_cap) grow_cache();
            cache[cache_len++] = kk;
        }
        ++next_k;
    }
    return cache[(size_t)n - 1];
}
