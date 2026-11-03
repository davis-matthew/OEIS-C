/*
 * A070865  Smallest prime such that the difference of successive terms is
 *          strictly increasing.
 * https://oeis.org/A070865
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = smallest prime such that differences a(i)-a(i‑1) are
 *            strictly increasing. Implementation uses memoisation and
 *            incremental construction. see OEIS A070865
 * Range:     u8 n <= 14, u16 n <= 154, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 65 terms through n = 8603 (2026-11-03)
 * Helpers:   is_prime_u128, is_prime_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
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

static seq_t *terms   = NULL;   /* 1‑based cache, index 0 unused */
static size_t terms_len = 0;     /* highest index currently stored (0 if empty) */
static size_t terms_cap = 0;     /* capacity of the array */

/* Ensure the cache can hold terms up to at least n (1‑based). */
static void ensure_terms(idx_t n)
{
    /* we need indices 0 … n, total size = n+1 */
    size_t needed = (size_t)n + 1;
    if (needed > terms_cap)
        ensure_capacity(needed, &terms_cap, (void**)&terms, sizeof(seq_t));
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    if (n < 1)
        return (seq_t)0;   /* undefined for n < 1, but safe guard */

    ensure_terms(n);

    /* If already computed, return the stored term. */
    if (terms_len >= (size_t)n)
        return terms[(size_t)n];

    /* Compute missing terms up to n. */
    for (idx_t idx = (idx_t)terms_len + 1; idx <= n; ++idx)
    {
        if (idx == 1)
        {
            terms[(size_t)idx] = (seq_t)2;
        }
        else if (idx == 2)
        {
            terms[(size_t)idx] = (seq_t)3;
        }
        else
        {
            /* Compute the previous gap: a(idx‑1) – a(idx‑2). */
            seq_t prev_gap = terms[(size_t)idx - 1] - terms[(size_t)idx - 2];

            /* The smallest candidate must satisfy diff > prev_gap,
               so start at a(idx‑1) + prev_gap + 1. */
            seq_t candidate = terms[(size_t)idx - 1] + prev_gap + (seq_t)1;

            /* Search for the first prime candidate. */
            while (!isPrimeGeneric(candidate))
                ++candidate;

            /* Store the found term. */
            terms[(size_t)idx] = candidate;
        }

        /* Update the length of the cache. */
        terms_len = (size_t)idx;
    }

    return terms[(size_t)n];
}
