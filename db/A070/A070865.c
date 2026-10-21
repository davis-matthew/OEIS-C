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
 * Helpers:   ensure_capacity, isPrimeGeneric
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/isPrimeGeneric.h"  /* isPrimeGeneric(seq_t) */
#include "../utils/ensure_capacity.h" /* ensure_capacity(size_t needed, size_t *cap, void **ptr, size_t elem_size) */

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
