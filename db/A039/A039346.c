/*
 * A039346  Numbers whose base-8 representation has the same nonzero number of
 *          2's and 7's.
 * https://oeis.org/A039346
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 18, u16 n <= 52, u32 n <= 52, u64 n <= 52, u128 n <= 52,
 *            big any n
 * Helpers:   has_eq_counts
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Algorithm: generate numbers whose base‑8 representation has an equal non‑zero
   number of digits 2 and 7.  The sequence is produced on demand and cached
   for subsequent calls. */
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/has_eq_counts.h"

/* Cached terms of the sequence.  The array grows lazily as needed. */
static seq_t *seq_cache     = NULL;   /* array of terms, 0‑based */
static size_t cache_size   = 0;       /* number of terms stored */
static size_t cache_cap    = 0;       /* capacity of seq_cache */
static seq_t next_candidate = 0;      /* last integer examined */

/* Ensure the cache contains at least n terms (n >= 1, 1‑based index). */
static void ensure_size(idx_t n)
{
    /* If already satisfied, nothing to do. */
    if (cache_size >= (size_t)n)
        return;

    /* Grow cache if necessary. */
    if (cache_size + (size_t)n > cache_cap) {
        size_t new_cap = (cache_cap == 0) ? 64 : cache_cap * 2;
        while (cache_size + (size_t)n > new_cap)
            new_cap *= 2;
        seq_t *new_mem = (seq_t *)realloc(seq_cache, new_cap * sizeof(seq_t));
        if (!new_mem) {
            /* Allocation failed; terminate program as sequence cannot be
               calculated further. */
            abort();
        }
        seq_cache = new_mem;
        cache_cap = new_cap;
    }

    /* Generate terms until we have n of them. */
    while (cache_size < (size_t)n) {
        /* Find next integer with equal counts of 2 and 7. */
        do {
            ++next_candidate;
        } while (!has_eq_counts(next_candidate));
        seq_cache[cache_size++] = next_candidate;
    }
}

/* Public function required by the framework. */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;   /* undefined for n = 0, return 0 as a safe fallback */
    ensure_size(n);
    return seq_cache[n - 1];  /* sequence is 1‑based, array is 0‑based */
}
