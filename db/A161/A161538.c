/*
 * A161538  Numbers n such that composite (n) + n is a prime, where composite
 *          (n) denotes the n-th composite number.
 * https://oeis.org/A161538
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the n-th integer k such that composite(k)+k is prime
 * Range:     u8 n <= 22, u16 n <= 57, u32 n <= 57, u64 n <= 57, u128 n <= 57,
 *            big any n
 * Validated: partial, pass, 57 terms through n = 57 (2026-11-03)
 * Helpers:   is_prime_u128, is_prime_u64, mem
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

/* Static cache of the sequence values */
static seq_t *cache     = NULL; /* cached a(i) values */
static size_t cache_size = 0;    /* number of values currently cached */
static size_t cache_cap   = 0;    /* capacity of the cache */

/* Current state for generating further terms */
static idx_t  next_candidate = 1;   /* current k (index of the composite) */
static seq_t  next_comp_val  = 4;   /* current composite number */

/* Ensure the cache has enough capacity */
static void ensure_cache_cap(size_t need)
{
    if (cache_cap >= need)
        return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 8;
    if (new_cap < need)
        new_cap = need;
    seq_t *tmp = mem_resize(seq_t, cache, new_cap);
    if (!tmp)
        abort();            /* allocation failure: stop execution */
    cache = tmp;
    cache_cap = new_cap;
}

/* Return the n-th term of sequence A161538 (1‑based index) */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;      /* undefined for n < 1, but guard against misuse */

    /* Generate terms until we have at least n values */
    while (cache_size < (size_t)n) {
        /* Test current candidate */
        seq_t sum = next_comp_val + (seq_t)next_candidate;
        if (isPrimeGeneric(sum)) {
            ensure_cache_cap(cache_size + 1);
            cache[cache_size++] = next_candidate;   /* store qualifying k */
        }

        /* Advance to next composite number and next candidate index */
        ++next_candidate;
        do {
            ++next_comp_val;
        } while (isPrimeGeneric(next_comp_val));
    }

    return cache[n - 1];
}
