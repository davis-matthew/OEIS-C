/*
 * A176782  a(n)=a(n-1)^3-2*a(n-2) with a(1)=1, a(2)=2.
 * https://oeis.org/A176782
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1)^3 - 2*a(n-2), a(1) = 1, a(2) = 2
 * Range:     u8 n <= 4, u16 n <= 4, u32 n <= 5, u64 n <= 5, u128 n <= 6,
 *            big any n
 * Helpers:   seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>                     /* for size_t */
#include "../utils/seq_realloc.h"       /* seq_realloc */

/* Static cache: cache[1] holds a(1), cache[2] holds a(2). */
static seq_t *cache = NULL;      /* pointer to the cache array   */
static size_t cap   = 0;          /* current allocated capacity   */
static idx_t  max_cached = 0;     /* highest n whose value is present */

/* Compute the n‑th term of the OEIS sequence A176782. */
seq_t a(idx_t n)
{
    /* Handle the defined offset and small indices directly. */
    if (n == 0) return (seq_t)0;  /* Undefined for n==0, but harmless */
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)2;

    /* Grow the cache if necessary. */
    if (n > max_cached) {
        /* Ensure space for indices 0 .. n; index 0 is unused. */
        size_t new_cap = (size_t)n + 1;
        seq_t *tmp;

        tmp = seq_realloc(cache, cap, new_cap);
        if (!tmp) abort();          /* allocation failure */
        cache = tmp;
        cap   = new_cap;

        /* Initialize base values if they were not set yet. */
        if (max_cached < 1) cache[1] = (seq_t)1;
        if (max_cached < 2) cache[2] = (seq_t)2;

        /* After initializing base values, mark them as cached. */
        if (max_cached < 2) max_cached = 2;

        /* Compute all missing terms up to n. */
        for (idx_t i = max_cached + 1; i <= n; ++i) {
            seq_t prev   = cache[i - 1];
            seq_t val    = prev * prev * prev - (seq_t)2 * cache[i - 2];
            cache[i] = val;
        }
        max_cached = n;
    }

    return cache[n];
}
