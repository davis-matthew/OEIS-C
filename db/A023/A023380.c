/*
 * A023380  a(n+1) = a(n) converted to base 7 from base 5 (written in base
 *          10).
 * https://oeis.org/A023380
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 10, u16 n <= 14, u32 n <= 18, u64 n <= 22, u128 n <= 26,
 *            big any n
 * Helpers:   base5_to_base7
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A023380
   a(1) = 5
   a(n+1) = interpret the base‑5 digits of a(n) as a base‑7 number
   (value returned in decimal).  */
#include <stdlib.h>
#include <stddef.h>
#include "../utils/base5_to_base7.h"

/* Compute a(n) with memoisation.  The static cache stores all terms
   that have already been calculated.  Subsequent calls reuse existing
   values. */
seq_t a(idx_t n)
{
    /* 0‑based index is unused; terms start at position 1. */
    if (n == 0)
        return 0;          /* undefined for the sequence, but harmless */

    static seq_t *cache = NULL;
    static idx_t cache_cap = 0;
    static idx_t cache_len = 0;   /* largest index already computed */

    if (cache == NULL) {
        cache_cap = 64;            /* initial capacity */
        cache = (seq_t *)calloc(cache_cap + 1, sizeof(seq_t));
        cache[1] = 5;              /* a(1) */
        cache_len = 1;
    }

    if (n > cache_cap) {
        cache_cap = n + 64;        /* grow with some spare space */
        cache = (seq_t *)realloc(cache, (cache_cap + 1) * sizeof(seq_t));
    }

    /* Extend the cache up to the requested index */
    for (idx_t i = cache_len + 1; i <= n; ++i)
        cache[i] = base5_to_base7(cache[i - 1]);

    cache_len = n;
    return cache[n];
}
