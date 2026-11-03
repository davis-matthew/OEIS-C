/*
 * A145332  Numbers Y such that 129*Y^2+43 is a square.
 * https://oeis.org/A145332
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 1, u32 n <= 2, u64 n <= 4, u128 n <= 9,
 *            big any n
 * Validated: standard, pass, 25 terms through n = 194 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(1)=53, a(2)=1786683, a(n+2)=33710*a(n+1)-a(n) */
#include <stddef.h>
#include "../utils/mem.h"
/* The following helper ensures that the array has room for
   indices 0 … n (index 0 is unused for convenience). */
#include <stdlib.h>

/* Make sure the cache is large enough */
static void ensure_cache(idx_t n, seq_t **cache_ptr, size_t *cap_ptr) {
    seq_t *cache = *cache_ptr;
    size_t cap = *cap_ptr;
    if (cap <= (size_t)n) {
        size_t new_cap = cap ? (cap << 1) : 16;
        while (new_cap <= (size_t)n)
            new_cap <<= 1;
        seq_t *tmp = mem_resize(seq_t, cache, new_cap);
        if (!tmp && new_cap > 0) {
            /* Allocation failed – abort the program. */
            exit(1);
        }
        if (tmp) {
            /* Initialize newly allocated slots to zero. */
            if (!cache) {
                for (size_t i = 0; i < new_cap; ++i)
                    tmp[i] = 0;
            } else {
                for (size_t i = cap; i < new_cap; ++i)
                    tmp[i] = 0;
            }
            *cache_ptr = tmp;
            *cap_ptr = new_cap;
        }
    }
}

/* Return the nth term of the sequence.  Indexing starts at 1. */
seq_t a(idx_t n)
{
    if (n < 1)
        return 0;                       /* undefined for zero or negative indices */

    static seq_t *cache = NULL;          /* 0‑based array, cache[0] unused */
    static size_t cache_cap = 0;         /* allocated size (in seq_t elements) */
    static idx_t  max_computed = 0;      /* highest index already filled */

    /* Make sure the array can hold index n */
    ensure_cache(n, &cache, &cache_cap);

    /* Initialise first two terms if we haven't yet */
    if (max_computed < 2) {
        cache[0] = 0;                     /* placeholder for unused index 0 */
        cache[1] = (seq_t)53;
        cache[2] = (seq_t)1786683;
        max_computed = 2;
    }

    /* Compute missing terms up to n */
    for (idx_t i = max_computed + 1; i <= n; ++i) {
        /* a(i) = 33710*a(i-1) - a(i-2) */
        cache[i] = (seq_t)33710 * cache[i - 1] - cache[i - 2];
    }
    max_computed = n;
    return cache[n];
}
