/*
 * A122522  a(n) = a(n - 2) + a(n - 8), with a(1) = ... = a(8) = 1.
 * https://oeis.org/A122522
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, less
 * Formula:   a(n) = a(n - 2) + a(n - 8), a(1) = ... = a(8) = 1
 * Range:     u8 n <= 40, u16 n <= 74, u32 n <= 142, u64 n <= 280,
 *            u128 n <= 556, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implements OEIS A122522 with memoisation */
/* The sequence is indexed from 1 (a(1) is the first term). */

#include <stdlib.h>

/* Cached sequence values: cache[0] unused, cache[1]..cache[n] used */
static idx_t cache_cap = 0;      /* number of allocated elements (incl. unused a[0]) */
static seq_t *cache = NULL;     /* pointer to cached values */

/* ------------------------------------------------------------ */
/* Return the n-th term of OEIS A122522                        */
/* ------------------------------------------------------------ */
seq_t a(idx_t n)
{
    /* Base values are all 1 for 1 ≤ n ≤ 8 */
    if (n <= 8)
        return (seq_t)1;

    /* Resize cache if necessary */
    if (cache_cap <= n) {
        /* Old capacity before resizing */
        size_t old_cap = (size_t)cache_cap;

        /* New capacity: need indices up to n (0..n inclusive) */
        size_t new_cap = (size_t)(n + 1);
        seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp)
            abort();                 /* allocation failed – terminate */
        cache = tmp;
        cache_cap = new_cap;

        /* Initialise the fixed initial terms (a(1)..a(8)) only if not already set */
        for (idx_t i = 1; i <= 8 && i < cache_cap; ++i)
            cache[i] = (seq_t)1;

        /* Compute missing values from the first index greater than the previous size */
        idx_t start = (old_cap > 8) ? old_cap : (idx_t)9;
        for (idx_t i = start; i <= n; ++i) {
            /* Recurrence: a(i) = a(i - 2) + a(i - 8) */
            cache[i] = cache[i - 2] + cache[i - 8];
        }
    }

    /* Now cache[n] is available */
    return cache[n];
}
