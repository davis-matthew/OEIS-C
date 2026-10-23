/*
 * A162485  a(1)=4, a(2)=6; for n > 2, a(n) = 2*a(n-1) + a(n-2) - 4*((n-1) mod
 *          2).
 * https://oeis.org/A162485
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 12, u32 n <= 25, u64 n <= 50, u128 n <= 100,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(1)=4, a(2)=6; for n>2,
   a(n)=2*a(n-1)+a(n-2)-4*((n-1) mod 2) */
#include <stdlib.h>

/* Computes the nth term of OEIS sequence A162485.
   Index n is 1‑based; n >= 1. */
seq_t a(idx_t n)
{
    /* Static cache of previously computed terms (1‑based). */
    static seq_t *cache     = NULL;
    static size_t cache_sz  = 0;
    static idx_t  last_idx  = 0;   /* largest index computed so far */

    /* Ensure sufficient storage. */
    if (cache == NULL || (size_t)n + 1 > cache_sz) {
        size_t new_sz = ((size_t)n + 1) * 2;     /* grow a bit to avoid frequent reallocs */
        seq_t *new_cache = (seq_t *)realloc(cache, new_sz * sizeof(seq_t));
        if (!new_cache) {
            /* allocation failed – free old memory if any */
            free(cache);
            cache     = NULL;
            cache_sz  = 0;
            last_idx  = 0;
            return (seq_t)0;
        }
        /* Zero-initialize the newly allocated part (not strictly necessary) */
        for (size_t i = cache_sz; i < new_sz; ++i)
            new_cache[i] = (seq_t)0;
        cache     = new_cache;
        cache_sz  = new_sz;
    }

    /* Initialise first two terms if not yet done. */
    if (last_idx == 0) {
        cache[1] = (seq_t)4;
        cache[2] = (seq_t)6;
        last_idx = 2;
    }

    /* If requested term already computed, return it. */
    if (n <= last_idx)
        return cache[(size_t)n];

    /* Compute missing terms up to n. */
    for (idx_t i = last_idx + 1; i <= n; ++i) {
        /* Subtract 4 when i is even (i.e., (i-1) mod 2 == 1). */
        if ((i & 1u) == 0)   /* i even */
            cache[(size_t)i] = (seq_t)2 * cache[(size_t)(i - 1)] +
                               cache[(size_t)(i - 2)] - (seq_t)4;
        else                 /* i odd */
            cache[(size_t)i] = (seq_t)2 * cache[(size_t)(i - 1)] +
                               cache[(size_t)(i - 2)];
    }
    last_idx = n;

    return cache[(size_t)n];
}
