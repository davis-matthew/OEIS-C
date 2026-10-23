/*
 * A105577  a(n) = 2*a(n-1) - 3*a(n-2) + 2*a(n-3) with a(0) = 1, a(1) = 5,
 *          a(2) = 6.
 * https://oeis.org/A105577
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = 2*a(n-1) - 3*a(n-2) + 2*a(n-3), a(0)=1, a(1)=5, a(2)=6
 * Range:     i8 n <= 11, i16 n <= 27, i32 n <= 43, i64 n <= 43, i128 n <= 43,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Static cache for previously computed terms */
static seq_t *cache = NULL;
static size_t  cap   = 0;   /* allocated capacity */
static size_t  sz    = 0;   /* number of stored terms */

/* Grow the cache to accommodate at least 'needed' entries */
static void ensure_capacity(size_t needed)
{
    if (cap >= needed)
        return;
    size_t newcap = cap ? (cap << 1) : 4;
    if (newcap < needed)
        newcap = needed;
    cache = (seq_t *)realloc(cache, newcap * sizeof(seq_t));
    cap   = newcap;
}

/* Compute a(n) for any n >= 0 */
seq_t a(idx_t n)
{
    /* Handle the three initial values directly */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)5;
    if (n == 2) return (seq_t)6;

    /* Initialise the cache on the first call */
    if (sz == 0)
    {
        ensure_capacity(4);
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)5;
        cache[2] = (seq_t)6;
        sz       = 3;
    }

    /* Return cached value if already computed */
    if (n < sz)
        return cache[(size_t)n];

    /* Expand the cache to fit the requested index */
    ensure_capacity((size_t)n + 1);

    /* Compute missing terms up to n */
    for (idx_t i = sz; i <= n; ++i)
    {
        seq_t val = 2 * cache[(size_t)(i - 1)]
                  - 3 * cache[(size_t)(i - 2)]
                  + 2 * cache[(size_t)(i - 3)];
        cache[(size_t)i] = val;
    }
    sz = (size_t)n + 1;
    return cache[(size_t)n];
}
