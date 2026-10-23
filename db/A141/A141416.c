/*
 * A141416  First differences of A133730.
 * https://oeis.org/A141416
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(0) = -1, a(1) = -1. * For n ≥ 1 even: a(2n) = -2 * a(2n‑1). *
 *            For all n: a(2n) + a(2n+1) = 2 * (-1)^(n+1). * Implements
 *            sequence A141416
 * Range:     i8 n <= 17, i16 n <= 33, i32 n <= 65, i64 n <= 129,
 *            i128 n <= 257, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;   /* cached values of the sequence */
static idx_t cache_size = 0;  /* number of values currently cached, equal to cache_size */

/* Ensure that cache contains values up to index n (inclusive). */
static void ensure_cache_upto(idx_t n)
{
    if (n < cache_size) return;          /* nothing to do */

    idx_t new_size = n + 1;               /* number of elements needed */
    seq_t *new_cache = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
    if (!new_cache) exit(EXIT_FAILURE);   /* abort on allocation failure */
    cache = new_cache;

    idx_t start = cache_size;             /* index of first element to compute */
    if (start == 0) {                     /* initialise the first element */
        cache[0] = (seq_t)-1;
        start = 1;
    }

    /* Compute new elements in order, using the recurrence */
    for (idx_t i = start; i <= n; ++i) {
        if ((i & 1) == 0) {               /* even index: a(2k) = -2 * a(2k‑1) */
            cache[i] = (seq_t)-2 * cache[i - 1];
        } else {                           /* odd index: a(2k+1) = 2 * (-1)^(k+1) – a(2k) */
            idx_t k = (i - 1) >> 1;       /* k = (i-1)/2 */
            int sign = ((k & 1) == 0) ? -1 : 1; /* (-1)^(k+1) */
            cache[i] = (seq_t)2 * sign - cache[i - 1];
        }
    }

    cache_size = new_size;                /* update cache size */
}

/* Public sequence function */
seq_t a(idx_t n)
{
    ensure_cache_upto(n);
    return cache[n];
}
