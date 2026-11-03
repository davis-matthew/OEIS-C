/*
 * A124520  Numbers k such that 14*k - 1 and 14*k + 1 are twin primes.
 * https://oeis.org/A124520
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 302, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 68 terms through n = 8597 (2026-11-03)
 * Helpers:   is_prime_u128, is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search for k such that 14*k-1 and 14*k+1 are twin primes. */
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

static seq_t *cache = NULL;          /* stored terms */
static size_t cache_size = 0;        /* number of cached terms */
static size_t cache_cap   = 0;        /* allocated capacity */
static uint64_t last_k    = 0;        /* last tested k value */

/* Grow the cache if necessary. */
static void cache_grow(void)
{
    size_t new_cap = cache_cap ? cache_cap * 2 : 8;
    seq_t *tmp = mem_resize(seq_t, cache, new_cap);
    if (!tmp) abort();
    cache = tmp;
    cache_cap = new_cap;
}

/* Append a value to the cache. */
static void cache_add(seq_t val)
{
    if (cache_size == cache_cap) cache_grow();
    cache[cache_size++] = val;
}

/* Return the n-th element of A124520 (1‑based index). */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* offset is 1; n==0 unused */

    /* Ensure we have enough cached terms. */
    while (cache_size < (size_t)n) {
        last_k++;                                   /* try next k */
        seq_t kseq = (seq_t)last_k;                 /* cast to seq_t for later ops */
        seq_t p1   = kseq * (seq_t)14 - (seq_t)1;   /* 14*k - 1 */
        seq_t p2   = kseq * (seq_t)14 + (seq_t)1;   /* 14*k + 1 */

        if (isPrimeGeneric(p1) && isPrimeGeneric(p2))
            cache_add(kseq);                      /* store valid k */
    }
    return cache[n - 1];
}
