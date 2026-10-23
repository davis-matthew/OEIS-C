/*
 * A155611  a(n) = 6^n - 3^n + 1.
 * https://oeis.org/A155611
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 12, u64 n <= 24, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: A155611
   Formula: a(n) = 6^n - 3^n + 1
   Recurrence: a(0)=1, a(1)=4,
                a(n) = 9*a(n-1) - 18*a(n-2) + 10   (n ≥ 2)
   This implementation uses a simple memoised recurrence.
   The cache is extended with standard realloc() on demand. */
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Memoised cache: static for all calls */
    static seq_t *cache = NULL;
    static size_t  cap   = 0;        /* number of elements stored */

    /* initialise cache with first two values */
    if (cache == NULL) {
        cap    = 2;
        cache  = (seq_t *)malloc(cap * sizeof(seq_t));
        if (!cache) abort();
        cache[0] = (seq_t)1;  /* a(0) */
        cache[1] = (seq_t)4;  /* a(1) */
    }

    /* If requested value already cached, just return it */
    if (n < (idx_t)cap) {
        return cache[(size_t)n];
    }

    /* Grow cache to accommodate n */
    size_t new_cap = (size_t)n + 1;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp) abort();            /* allocation failed */
    cache = tmp;

    /* Compute missing terms using the recurrence */
    for (size_t i = cap; i < new_cap; ++i) {
        seq_t term = (seq_t)9 * cache[i - 1];
        term -= (seq_t)18 * cache[i - 2];
        term += (seq_t)10;
        cache[i] = term;
    }

    cap = new_cap;
    return cache[(size_t)n];
}
