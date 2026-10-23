/*
 * A047854  a(n) = A047848(6, n).
 * https://oeis.org/A047854
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (9^n + 7)/8 a(0) = 1, a(n) = 9*a(n-1) - 7 for n >= 1
 *            (unrolled recurrence)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 11, u64 n <= 21, u128 n <= 41,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

static seq_t *cache = NULL;      /* dynamically growable array of values   */
static size_t cache_cap = 0;     /* allocated capacity (number of elements) */
static idx_t   cache_max_n = 0;  /* highest index already computed          */

/* Ensure that cache contains values up to index n. */
static void ensure_cache(idx_t n)
{
    /* Allocate or grow the underlying array if necessary. */
    if (cache == NULL) {                 /* first call */
        cache_cap = (size_t)n + 1;
        cache = (seq_t *)malloc(sizeof(seq_t) * cache_cap);
        if (cache == NULL) abort();      /* fatal error */
        cache[0] = (seq_t)1;             /* a(0) */
        cache_max_n = 0;
        if (n >= 1) {                    /* initialise a(1) if needed */
            cache[1] = (seq_t)2;         /* a(1) */
            cache_max_n = 1;
        }
    } else if ((size_t)n >= cache_cap) { /* extend capacity */
        size_t new_cap = (size_t)n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (tmp == NULL) abort();        /* fatal error */
        cache = tmp;
        cache_cap = new_cap;
        /* a(0) and a(1) already in place from earlier calls */
    }

    /* Compute any missing terms using the recurrence. */
    if (cache_max_n < n) {
        seq_t nine = (seq_t)9;
        seq_t seven = (seq_t)7;
        for (idx_t i = cache_max_n + 1; i <= n; ++i) {
            cache[i] = nine * cache[i - 1] - seven;
        }
        cache_max_n = n;
    }
}

/* Public entry point.  Returns a(n) for the sequence A047854. */
seq_t a(idx_t n)
{
    ensure_cache(n);
    return cache[n];
}
