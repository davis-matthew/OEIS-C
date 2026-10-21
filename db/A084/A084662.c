/*
 * A084662  a(1) = 4; a(n) = a(n-1) + gcd(a(n-1), n).
 * https://oeis.org/A084662
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1) + gcd(a(n-1), n), a(1) = 4
 * Range:     u8 n <= 100, u16 n <= 9475, u32 n <= 9475, u64 n <= 9475,
 *            u128 n <= 9475, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Computes term a(n) of OEIS sequence A084662.
    The function uses a static cache so that calls in any order are
    handled efficiently.  The recurrence is evaluated iteratively, and
    intermediate values are kept in a dynamic array that grows
    as larger indices are requested. */

#include <stdlib.h>   /* malloc, realloc */

/* Euclidean algorithm for unsigned seq_t values */
static seq_t seq_gcd(seq_t a, seq_t b)
{
    while (b != (seq_t)0) {
        seq_t t = a % b;
        a = b;
        b = t;
    }
    return a;
}

/* Static cache of computed terms */
static seq_t  *cache      = NULL;     /* array of terms */
static size_t  cache_size = 0;        /* allocated array size (in elements) */
static idx_t   cache_len  = 0;        /* highest index cached (0 if none) */

seq_t a(idx_t n)
{
    if (n == 0)          /* use 0 as a sentinel for undefined indices */
        return (seq_t)0;

    /* Initialize cache on first call */
    if (cache_len == 0) {
        cache_size = 2;                     /* indices 0 and 1 */
        cache = (seq_t *)malloc(cache_size * sizeof(seq_t));
        cache[0] = (seq_t)0;                /* unused, for convenience */
        cache[1] = (seq_t)4;                /* a(1) = 4   */
        cache_len = 1;
    }

    /* Grow array if necessary */
    if ((idx_t)cache_size <= n) {
        size_t new_size = (size_t)n + 2;    /* +2 to allow indices 0..n */
        seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (!tmp) {                         /* allocation failed: return 0 */
            free(cache);
            cache = NULL;
            cache_len = 0;
            cache_size = 0;
            return (seq_t)0;
        }
        cache = tmp;
        cache_size = new_size;
    }

    /* Compute missing terms */
    if (n > cache_len) {
        for (idx_t i = cache_len + 1; i <= n; ++i) {
            seq_t prev = cache[i - 1];
            seq_t g = seq_gcd(prev, (seq_t)i);
            cache[i] = prev + g;
        }
        cache_len = n;
    }

    return cache[n];
}
