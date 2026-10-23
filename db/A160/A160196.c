/*
 * A160196  Numerator of Hermite(n, 13/28).
 * https://oeis.org/A160196
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, frac
 * Formula:   a(n) = 14^n * Hermite(n, 13/28) * Recurrence: a(0) = 1, a(1) =
 *            13, * a(n+1) = 13 * a(n) - 392 * n * a(n-1) * for n >= 1
 * Range:     i8 n <= 1, i16 n <= 3, i32 n <= 6, i64 n <= 11, i128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h> /* for malloc, realloc, exit */

static seq_t *cache = NULL;          /* dynamically allocated cache of terms */
static idx_t  cache_size = 0;        /* allocated number of elements */
static idx_t  last_computed = 0;     /* highest index currently stored */
static int    cache_initialized = 0; /* flag whether cache[0] and cache[1] set */

/* enlarge the cache so that cache[n] is valid (n is idx_t) */
static void ensure_capacity(idx_t n)
{
    if (cache_size > n) return;
    idx_t new_cap = cache_size ? cache_size * 2 : 1;
    while (new_cap <= n) new_cap *= 2;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp) exit(1);                 /* allocation failure */
    cache = tmp;
    cache_size = new_cap;
}

seq_t a(idx_t n)
{
    ensure_capacity(n);

    /* initialise a(0) and a(1) on first use */
    if (!cache_initialized) {
        cache[0] = (seq_t)1;                   /* a(0) = 1 */
        if (n >= 1) cache[1] = (seq_t)13;      /* a(1) = 13 */
        last_computed = n >= 1 ? 1 : 0;
        cache_initialized = 1;
    }

    /* If the requested term is already cached, return it */
    if (n <= last_computed) return cache[n];

    /* Compute any missing terms up to n */
    for (idx_t i = last_computed + 1; i <= n; ++i) {
        seq_t term =
            (seq_t)13 * cache[i - 1]
            - (seq_t)392 * (i - 1) * cache[i - 2];
        cache[i] = term;
    }
    last_computed = n;
    return cache[n];
}
