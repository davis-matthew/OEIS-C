/*
 * A158538  a(n) = Hermite(n,12).
 * https://oeis.org/A158538
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, sign
 * Formula:   a(n) = 24*a(n-1) - 2*(n-1)*a(n-2), a(0)=1, a(1)=24
 * Range:     i8 n <= 1, i16 n <= 3, i32 n <= 6, i64 n <= 13, i128 n <= 28,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include <assert.h>

static seq_t *cache = NULL;        /* array holding computed terms */
static size_t  cache_cap = 0;      /* current allocated capacity in elements */
static idx_t   max_computed = 0;   /* highest index for which cache[index] is valid */

/* Ensure the cache can hold indices up to n (inclusive). */
static void ensure_cache(idx_t n)
{
    if ((size_t)(n + 1) <= cache_cap)
        return;

    size_t new_cap = cache_cap ? cache_cap * 2 : 4;
    while (new_cap <= (size_t)n)
        new_cap *= 2;

    seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!new_cache && new_cap > 0)
        abort();                     /* allocation failed – terminate */

    cache = new_cache;
    cache_cap = new_cap;
}

/* Initialise the first two terms if they have not yet been set. */
static void init_first_terms(void)
{
    if (max_computed >= 1)
        return;

    ensure_cache(1);
    cache[0] = (seq_t)1;
    cache[1] = (seq_t)24;
    max_computed = 1;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;              /* a(0) is defined independently */

    if (max_computed == 0)
        init_first_terms();

    if (n <= max_computed)
        return cache[n];

    ensure_cache(n);

    /* Compute missing terms using the recurrence: a(n) = 24*a(n-1) - 2*(n-1)*a(n-2) */
    for (idx_t i = max_computed + 1; i <= n; ++i)
    {
        seq_t term = (seq_t)2 * (seq_t)(i - 1);
        cache[i] = (seq_t)24 * cache[i - 1] - term * cache[i - 2];
    }

    max_computed = n;
    return cache[n];
}
