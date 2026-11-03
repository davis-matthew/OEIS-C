/*
 * A032619  Numbers k such that k concatenated with k+4 is a prime.
 * https://oeis.org/A032619
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 12, u32 n <= 3026, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 70 terms through n = 8432 (2026-11-03)
 * Helpers:   is_prime_u128, is_prime_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A032619: numbers k such that concatenation of k with k+4 is prime. */

#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>

/* Utility functions (provided by the build system) */
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

static seq_t concat_parts(idx_t a, idx_t b)
{
    seq_t part_a = (seq_t)a;
    seq_t part_b = (seq_t)b;
    seq_t pow = 1;
    seq_t tmp = part_b;
    /* pow = 10^(number of decimal digits of b) */
    while (tmp > 0) {
        pow *= 10;
        tmp /= 10;
    }
    return part_a * pow + part_b;
}

/* Helper allocation for seq_t (arbitrary‑precision or builtin integer) */
static seq_t* seq_alloc(size_t cap) {
#ifdef __cplusplus
    return new seq_t[cap];
#else
    return (seq_t*)malloc(cap * sizeof(seq_t));
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

/* Static cache for computed terms */
static seq_t *cache = NULL;
static size_t cache_len = 0;   /* number of terms currently stored */
static size_t cache_cap = 0;   /* allocated capacity of the cache */

/* The smallest candidate k not yet examined */
static idx_t next_k = 1;

/* Ensure the cache has at least 'needed' slots.  Reallocate if necessary. */
static void ensure_cache_cap(size_t needed) {
    if (needed <= cache_cap)
        return;
    size_t new_cap = cache_cap ? cache_cap * 2U : 16U;
    if (needed > new_cap)
        new_cap = needed;
    seq_t *tmp = seq_realloc(cache, cache_cap, new_cap);
    if (tmp == NULL) {
        /* In the unlikely event of a memory allocation failure we abort. */
        abort();
    }
    cache = tmp;
    cache_cap = new_cap;
}

/* Return the n‑th term of the sequence (n >= 1). */
seq_t a(idx_t n) {
    if (n == 0)
        return 0;               /* offset is 1 – returning 0 for n==0 is harmless */

    /* If the request is within the already cached range, return it immediately. */
    if ((size_t)n <= cache_len)
        return cache[(size_t)n - 1U];

    /* Extend the cache until we have computed the n‑th term. */
    while (cache_len < n) {
        idx_t k = next_k;
        while (true) {
            seq_t concat = concat_parts(k, k + 4);
            if (isPrimeGeneric(concat)) {
                /* Append k to the cache. */
                ensure_cache_cap(cache_len + 1);
                cache[cache_len] = (seq_t)k;
                cache_len++;

                /* The next candidate will be the following integer. */
                next_k = k + 1;
                break;           /* proceed to the next term */
            }
            ++k;                /* try the next integer */
        }
    }

    return cache[(size_t)n - 1U];
}
