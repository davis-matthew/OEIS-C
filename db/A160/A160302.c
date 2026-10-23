/*
 * A160302  Numerator of Hermite(n, 4/31).
 * https://oeis.org/A160302
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, frac
 * Formula:   a(n+2) = 8*a(n+1) - 1922*(n+1)*a(n); a(0) = 1; a(1) = 8
 * Range:     i8 n <= 1, i16 n <= 2, i32 n <= 5, i64 n <= 9, i128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Cache for memoisation – shared across calls */
static seq_t *cache = NULL;
static idx_t cache_size = 0;      /* allocated slots */
static idx_t max_computed = 0;    /* highest n that has been computed */

/* Grow the cache to at least 'needed' slots (0‑based indexing). */
static void
ensure_capacity(idx_t needed)
{
    if (needed < cache_size)
        return;
    idx_t new_cap = cache_size ? cache_size * 2 : 8;
    if (new_cap <= needed)
        new_cap = needed + 1;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp) abort();          /* out of memory – fatal */
    cache = tmp;
    cache_size = new_cap;
}

seq_t
a(idx_t n)
{
    /* Initialise base cases on first call */
    if (!cache) {
        ensure_capacity(2);      /* at least two slots for a(0) and a(1) */
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)8;
        max_computed = 1;
    }

    /* If value already computed, return it immediately */
    if (n <= max_computed)
        return cache[n];

    /* Compute new terms up to n, expanding the cache as needed */
    for (idx_t i = max_computed + 1; i <= n; ++i) {
        seq_t a_im1 = cache[i - 1];
        seq_t a_im2 = cache[i - 2];
        seq_t term  = (seq_t)(i - 1);          /* (n+1) in the recurrence */
        seq_t val   = 8 * a_im1 - 1922 * term * a_im2;
        if (i == cache_size)          /* allocate one more slot if needed */
            ensure_capacity(i + 1);
        cache[i] = val;
    }
    max_computed = n;
    return cache[n];
}
