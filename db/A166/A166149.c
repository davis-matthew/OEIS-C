/*
 * A166149  a(n) = (5^n + 10*(-6)^n)/11.
 * https://oeis.org/A166149
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, sign
 * Formula:   not stated
 * Range:     i8 n <= 2, i16 n <= 5, i32 n <= 12, i64 n <= 24, i128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = (5^n + 10*(-6)^n)/11
 * Recurrence: a(0)=1, a(1)=-5, a(n)=30*a(n-2)-a(n-1)
 */
#include <stdlib.h>

/* Note: seq_t and idx_t are provided by the build environment. */
extern seq_t a(idx_t n);

/* Cache for previously computed values */
static seq_t *cache = NULL;   /* pointer to cache array */
static size_t cache_cap = 0;  /* number of stored elements */

/* Ensure the cache contains values up to index n. */
static void ensure_cache(idx_t n)
{
    /* Keep track of the first index that needs to be computed. */
    size_t start;

    /* Initialisation of the cache. */
    if (cache == NULL)
    {
        cache_cap = 2;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        cache[0] = (seq_t)1;        /* a(0) */
        cache[1] = (seq_t)-5;       /* a(1) */
        start   = 2;
    }
    else
    {
        start = cache_cap;  /* first index not yet computed */
    }

    /* Grow the cache array if necessary. */
    if ((size_t)(n + 1) > cache_cap)
    {
        size_t new_cap = (size_t)(n + 1);
        seq_t *new_cache = (seq_t *)malloc(new_cap * sizeof(seq_t));
        for (size_t i = 0; i < cache_cap; ++i)
            new_cache[i] = cache[i];
        free(cache);
        cache = new_cache;
        cache_cap = new_cap;
    }

    /* Compute missing terms using the recurrence. */
    for (size_t i = start; i <= (size_t)n; ++i)
    {
        /* All operations are performed in seq_t to avoid overflow of
           intermediate results in the arbitrary‑precision build. */
        cache[i] = (seq_t)(30 * cache[i - 2] - cache[i - 1]);
    }
}

seq_t a(idx_t n)
{
    /* Handle the two initial values directly. */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)-5;

    /* Compute or retrieve the requested term. */
    ensure_cache(n);
    return cache[(size_t)n];
}
