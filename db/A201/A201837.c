/*
 * A201837  G.f.: real part of 1/(1 - i*x - i*x^2) where i=sqrt(-1).
 * https://oeis.org/A201837
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = -a(n-2) - 2*a(n-3) - a(n-4) Initial values: a(0)=1,
 *            a(1)=0, a(2)=-1, a(3)=-2
 * Range:     i8 n <= 15, i16 n <= 29, i32 n <= 60, i64 n <= 121,
 *            i128 n <= 242, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation uses a dynamically sized static cache.  The cache
   holds all computed terms up to the largest index that has ever been
   requested; each new call expands the cache as necessary.  The
   recurrence is evaluated directly using the cached values. */

#include <stdint.h>   /* for uint64_t (idx_t) */
#include <stdlib.h>   /* for malloc / realloc */

/* static cache state */
static seq_t *a_cache   = NULL;  /* pointer to array of terms */
static size_t a_size   = 0;      /* number of terms currently stored */
static size_t a_cap    = 0;      /* allocated capacity */

/* Return the n‑th term of A201837 */
seq_t a(idx_t n)
{
    /* Ensure the cache has at least the first four terms on first call */
    if (a_cache == NULL) {
        a_cap = 4;
        a_cache = (seq_t *)realloc(NULL, a_cap * sizeof(seq_t));
        if (a_cache == NULL) return (seq_t)0; /* allocation failed – unlikely */
        a_cache[0] = (seq_t)1;
        a_cache[1] = (seq_t)0;
        a_cache[2] = (seq_t)-1;
        a_cache[3] = (seq_t)-2;
        a_size  = 4;
    }

    /* If the requested index is already cached, return it immediately */
    if ((size_t)n < a_size)
        return a_cache[(size_t)n];

    /* Determine new required size and grow the cache if necessary */
    size_t required = (size_t)n + 1;
    if (required > a_cap) {
        /* Double the capacity until it fits the required size */
        while (a_cap < required)
            a_cap = a_cap * 2 + 1;   /* +1 to handle a_cap==0 case */
        seq_t *tmp = (seq_t *)realloc(a_cache, a_cap * sizeof(seq_t));
        if (tmp == NULL) return (seq_t)0;   /* allocation failed – unlikely */
        a_cache = tmp;
    }

    /* Compute missing terms using the recurrence */
    for (idx_t i = a_size; i <= n; ++i) {
        /* recurrence: a(n) = -a(n-2) - 2*a(n-3) - a(n-4) */
        a_cache[(size_t)i] =
            -(a_cache[(size_t)(i - 2)]) -
            (seq_t)2 * a_cache[(size_t)(i - 3)] -
            a_cache[(size_t)(i - 4)];
    }

    a_size = required;
    return a_cache[(size_t)n];
}
