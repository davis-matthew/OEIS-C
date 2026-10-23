/*
 * A160003  Numerator of Hermite(n, 1/25).
 * https://oeis.org/A160003
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, frac
 * Formula:   a(n) = 25^n * Hermite(n, 1/25)
 * Range:     i8 n <= 1, i16 n <= 3, i32 n <= 5, i64 n <= 10, i128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0) = 1, a(1) = 2 */
/*             a(n) = 2*a(n-1) - 1250*(n-1)*a(n-2)  for n >= 2 */

#include <stdlib.h>

/* Static cache of computed terms.  The cache grows on demand. */
static seq_t *cache = NULL;        /* array of computed a(n) values      */
static idx_t  cached_len = 0;      /* current allocated length (in elements) */
static idx_t  max_cached = 0;      /* largest n that has been computed   */

/* Return a(n) for any n >= 0.  The function is safe for repeated and
   non‑monotonic calls.  It uses the closed form recurrence above. */
seq_t a(idx_t n)
{
    /* Initialise the cache if it does not yet exist.  Allocate space
       for a(0) and a(1) and store the initial values. */
    if (cached_len == 0) {
        seq_t *tmp = (seq_t *)realloc(cache, 2 * sizeof(seq_t));
        cache = tmp ? tmp : cache;          /* keep old pointer if realloc failed (unlikely) */
        cached_len = 2;
        cache[0] = (seq_t)1;                /* a(0) */
        cache[1] = (seq_t)2;                /* a(1) */
        max_cached = 1;
    }

    /* If the requested index is already cached, return the value. */
    if (n <= max_cached)
        return cache[n];

    /* Grow the cache to hold terms up to n. */
    if (n + 1 > cached_len) {
        seq_t *tmp = (seq_t *)realloc(cache, (n + 1) * sizeof(seq_t));
        cache = tmp ? tmp : cache;          /* keep old pointer if realloc failed */
        cached_len = n + 1;
    }

    /* Compute missing terms using the recurrence. */
    for (idx_t i = max_cached + 1; i <= n; ++i) {
        /* a(i) = 2*a(i-1) - 1250*(i-1)*a(i-2) */
        seq_t t1 = cache[i - 1] * 2;         /* 2*a(i-1) */
        seq_t t2 = cache[i - 2];
        t2 *= (seq_t)1250 * (seq_t)(i - 1);  /* 1250*(i-1)*a(i-2) */
        cache[i] = t1 - t2;
    }

    max_cached = n;
    return cache[n];
}
