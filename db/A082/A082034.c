/*
 * A082034  a(n) = (4*n + 1)*n!.
 * https://oeis.org/A082034
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (4*n + 1) * n!
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 11, u64 n <= 19, u128 n <= 32,
 *            big any n
 * Validated: standard, pass, 38 terms through n = 372 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Computes A082034 for any non‑negative n.
    The implementation caches computed factorials so that repeated
    calls are fast even if the indices are not monotonic.  */

#include <stdlib.h>   /* malloc, realloc */
#include "../utils/mem.h"

static seq_t *fact_cache = NULL;  /* factorial cache, fact_cache[i] == i! */
static idx_t fact_cache_size = 0; /* number of factorials cached (i.e. cache size) */

seq_t a(idx_t n)
{
    /* Ensure the cache array exists. */
    if (fact_cache == NULL) {
        fact_cache = mem_new(seq_t, 1);   /* array of size 1 for 0! */
        if (!fact_cache) return 0;                     /* allocation failed */
        fact_cache[0] = 1;                             /* 0! = 1 */
        fact_cache_size = 1;
    }

    /* Grow the cache if we need factorials up to n. */
    if (n >= fact_cache_size) {
        size_t new_size = (size_t)n + 1;               /* new length, fits size_t for n <= 10000 */
        seq_t *tmp = mem_resize(seq_t, fact_cache, new_size);
        if (!tmp) return 0;                            /* allocation failed */
        fact_cache = tmp;

        /* Compute missing factorials. */
        for (idx_t i = fact_cache_size; i <= n; ++i) {
            fact_cache[(size_t)i] = fact_cache[(size_t)(i - 1)] * (seq_t)i;
        }
        fact_cache_size = new_size;
    }

    /* Compute multiplier (4*n + 1). */
    seq_t mult = (seq_t)n;
    mult *= 4;
    mult += 1;

    /* Return (4*n + 1) * n! */
    return fact_cache[(size_t)n] * mult;
}
