/*
 * A213169  a(n) = n! + n + 1.
 * https://oeis.org/A213169
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n! + n + 1
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 12, u64 n <= 20, u128 n <= 34,
 *            big any n
 * Helpers:   ensure_capacity
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes a(n) for the OEIS sequence A213169, using a recurrence relation
   a(0) = 2
   a(n) = n * a(n-1) - n * (n-1) + 1   for n ≥ 1
   The function caches computed values so that repeated calls are efficient. */

#include <stddef.h>
#include <stdlib.h>
#include "../utils/ensure_capacity.h"

seq_t a(idx_t n)
{
    /* static cache for previously computed values */
    static seq_t *cache = NULL;
    static size_t size  = 0;  /* number of elements in cache */
    static size_t cap   = 0;  /* allocated capacity for cache */

    /* ensure space for value at index n */
    if (n + 1 > size) {
        /* expand cache up to n */
        ensure_capacity(n + 1, &cap,
                        (void **)&cache, sizeof(seq_t));

        /* base case: compute a(0) if not yet present */
        if (size == 0) {
            cache[0] = 2;          /* 0! + 0 + 1 = 2 */
            size = 1;
        }

        /* compute remaining terms up to n */
        for (idx_t i = size; i <= n; ++i) {
            seq_t term = (seq_t)i * cache[i - 1];
            term -= (seq_t)i * (i - 1);
            term += 1;
            cache[i] = term;
        }
        size = n + 1;  /* cache now contains [0..n] */
    }

    return cache[n];
}
