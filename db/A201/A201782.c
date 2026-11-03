/*
 * A201782  Primes of the form 3n^2 - 10.
 * https://oeis.org/A201782
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = the nth prime of the form 3k^2 – 10 (k counted from 1)
 * Range:     u8 n <= 4, u16 n <= 24, u32 n <= 2466, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Validated: standard, pass, 50 terms through n = 4788 (2026-11-03)
 * Helpers:   is_prime_u128, is_prime_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
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

/* static cache for the sequence */
static seq_t *cache = NULL;
static size_t cache_len = 0, cache_cap = 0;
static idx_t next_k = 1;                         /* k to test next */

/* ensure cache capacity */
static void grow_cache(size_t new_len)
{
    if (cache_cap < new_len) {
        size_t new_cap = cache_cap ? cache_cap : 256;
        while (new_cap < new_len)
            new_cap *= 2;
        /* seq_realloc moves contents and frees old block */
        cache = seq_realloc(cache, cache_cap, new_cap);
        if (!cache)
            abort();
        cache_cap = new_cap;
    }
}

/* sequence generator */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                 /* undefined for n<1, return 0 */

    /* generate primes until we have at least n elements */
    while (cache_len < (size_t)n) {
        seq_t nk = (seq_t)next_k;
        seq_t val = (seq_t)3 * nk * nk - (seq_t)10;
        if (isPrimeGeneric(val)) {
            grow_cache(cache_len + 1);
            cache[cache_len++] = val;
        }
        ++next_k;
    }
    return cache[n - 1];
}
