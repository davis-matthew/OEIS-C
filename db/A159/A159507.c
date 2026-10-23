/*
 * A159507  Numerator of Hermite(n, 1/14).
 * https://oeis.org/A159507
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, frac
 * Formula:   a(n) = a(n-1) - 98*(n-1)*a(n-2); a(0) = 1; a(1) = 1;
 * Range:     i8 n <= 2, i16 n <= 4, i32 n <= 7, i64 n <= 13, i128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;
static size_t cache_size = 0;
static idx_t last_computed = (idx_t)-1;   /* sentinel for uninitialized */

/* Ensure that cache has capacity for at least “needed” elements. */
static void expand_cache(size_t needed)
{
    if (needed <= cache_size)
        return;
    seq_t *new_cache = (seq_t *)realloc(cache, needed * sizeof(seq_t));
    if (!new_cache)
        abort();                 /* allocation failed */
    cache = new_cache;
    cache_size = needed;
    /* Newly allocated elements are uninitialized; they will be filled in */
}

/* Return the nth term of A159507.  Supports arbitrary ordering of calls. */
seq_t a(idx_t n)
{
    /* Initialize the first two terms once. */
    if (last_computed == (idx_t)-1) {
        expand_cache(2);         /* need indices 0 and 1 */
        cache[0] = 1;
        cache[1] = 1;
        last_computed = 1;
    }

    /* If we already have the value, return it immediately. */
    if (n <= last_computed)
        return cache[n];

    /* Compute missing terms up to n. */
    size_t need = (size_t)(n + 1);
    expand_cache(need);

    idx_t i;
    for (i = last_computed + 1; i <= n; ++i) {
        seq_t term = cache[i - 2];
        term *= (seq_t)98;            /* 98 * a(i-2) */
        term *= (seq_t)(i - 1);       /* 98*(i-1)*a(i-2) */
        cache[i] = cache[i - 1] - term;
    }
    last_computed = n;
    return cache[n];
}
