/*
 * A140127  The first five terms are 1, after that,
 *          a(n)=a(n-1)^2-a(n-2)-a(n-3)-a(n-4)-a(n-5).
 * https://oeis.org/A140127
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   not stated
 * Range:     i8 n <= 8, i16 n <= 9, i32 n <= 10, i64 n <= 11, i128 n <= 12,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(1)=1, a(2)=1, a(3)=1, a(4)=1, a(5)=1
   a(n)=a(n-1)^2-a(n-2)-a(n-3)-a(n-4)-a(n-5)   (n≥6) */

#include <stdlib.h>   /* malloc, realloc, exit */
#include <stddef.h>   /* size_t */

static seq_t *cache = NULL;     /* 1‑based array of computed terms */
static size_t cache_cap = 0;    /* allocated size (number of elements) */
static idx_t  last_computed = 0;/* largest n for which cache[n] is valid */

/* Ensure cache has room for indices up to n (inclusive).  Grow geometrically. */
static void ensure_capacity(idx_t n)
{
    if ((size_t)n + 1 <= cache_cap)
        return;
    size_t new_cap = cache_cap ? cache_cap : 16;
    while (new_cap <= (size_t)n + 1)
        new_cap <<= 1;

    seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!new_cache && new_cap > 0)
    {
        /* Out of memory – terminate the program. */
        exit(EXIT_FAILURE);
    }
    cache      = new_cache;
    cache_cap  = new_cap;
}

/* Return a(n) for n ≥ 1.  The function is memoised so that any order
   of calls is efficient. */
seq_t a(idx_t n)
{
    if (n < 1)
        return 0;   /* undefined for n < 1 – return a safe default */

    /* Initialise the first five terms on first entry */
    if (last_computed == 0)
    {
        ensure_capacity(5);
        cache[1] = (seq_t)1;
        cache[2] = (seq_t)1;
        cache[3] = (seq_t)1;
        cache[4] = (seq_t)1;
        cache[5] = (seq_t)1;
        last_computed = 5;
    }

    /* If the requested term is already computed, return it */
    if (n <= last_computed)
        return cache[n];

    /* Compute terms up to n iteratively, using the recurrence */
    ensure_capacity(n);
    for (idx_t i = last_computed + 1; i <= n; ++i)
    {
        /* a(i) = a(i-1)^2 - a(i-2) - a(i-3) - a(i-4) - a(i-5) */
        seq_t val = cache[i-1] * cache[i-1];
        val = val - cache[i-2];
        val = val - cache[i-3];
        val = val - cache[i-4];
        val = val - cache[i-5];
        cache[i] = val;
    }
    last_computed = n;
    return cache[n];
}
