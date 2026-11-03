/*
 * A121250  Numbers n such that n^2 + 14 is prime.
 * https://oeis.org/A121250
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the n-th integer m such that m^2 + 14 is prime
 * Range:     u8 n <= 2, u16 n <= 13, u32 n <= 1518, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Validated: standard, pass, 67 terms through n = 4975 (2026-11-03)
 * Helpers:   is_prime_u128, is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The function builds terms lazily and caches them. */
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

/* Static cache of found terms. */
static seq_t *cache = NULL;      /* array of a(1), a(2), ...          */
static size_t cache_size = 0;    /* number of terms currently cached   */
static size_t cache_cap  = 0;    /* allocated capacity (in seq_t)      */
static idx_t next_candidate = 1; /* smallest n not yet examined        */

/* Ensure the cache can hold at least 'needed' items. */
static void grow_cache(size_t needed)
{
    if (needed <= cache_cap) return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 16;
    if (new_cap < needed) new_cap = needed;
    cache = mem_resize(seq_t, cache, new_cap);
    cache_cap = new_cap;
}

/* Test whether n^2 + 14 is prime. */
static bool test_candidate(idx_t n)
{
    seq_t nn = (seq_t)n;
    seq_t sq = nn * nn + (seq_t)14;   /* n^2 + 14, not n*(n+14) */
    return isPrimeGeneric(sq);
}

/* Return the n-th element of the sequence (offset 1).  */
seq_t a(idx_t n)
{
    if (n == 0) return 0;  /* undefined for offset 1 – sentinel */
    /* Generate terms until we have at least n of them. */
    while (cache_size < n) {
        /* Find the next candidate that satisfies the condition. */
        while (1) {
            if (test_candidate(next_candidate)) {
                grow_cache(cache_size + 1);
                cache[cache_size++] = (seq_t)next_candidate;
                next_candidate++;
                break;
            }
            next_candidate++;
        }
    }
    return cache[n - 1];
}
