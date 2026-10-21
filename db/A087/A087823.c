/*
 * A087823  a(1) = a(2) = 1; for n > 2, a(n) = a(a(a(n-2))) + a(n-a(n-2)).
 * https://oeis.org/A087823
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(1) = a(2) = 1; for n > 2, a(n) = a(a(a(n-2))) + a(n-a(n-2))
 * Range:     u8 n <= 77, u16 n <= 77, u32 n <= 77, u64 n <= 77, u128 n <= 77,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 1-based indexing (a(1) is the first displayed term). */

#include <stddef.h>
#include <stdlib.h>

/* Static cache for computed terms, 1-based.  cache[0] is unused. */
static seq_t *cache = NULL;
static size_t cache_cap = 0;          /* current allocated capacity (elements) */
static idx_t  cached_max = 0;         /* index of largest computed term */

/* Ensure the cache can hold indices up to n (inclusive).  */
static void
ensure_capacity(idx_t n)
{
    if (n + 1 <= cache_cap)
        return;                     /* already large enough */

    size_t new_cap = cache_cap ? cache_cap * 2 : 16u;
    while (new_cap <= (size_t)(n + 1))
        new_cap <<= 1;                /* grow until big enough */

    seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!new_cache)
        abort();                     /* Out of memory */
    cache = new_cache;
    cache_cap = new_cap;

    /* cache[0] is unused, but initialise it to 0 just for safety. */
    if (cached_max == 0)
        cache[0] = 0;
}

/* Compute the sequence up to index n (inclusive).  */
static void
populate_to(idx_t n)
{
    for (idx_t i = cached_max + 1; i <= n; ++i)
    {
        if (i == 1 || i == 2)
        {
            cache[i] = 1;
        }
        else
        {
            /* a(i-2) */
            seq_t val_n2 = cache[i - 2];

            /* a(a(a(i-2))) */
            idx_t idx1 = (idx_t)val_n2;           /* a(i-2) */
            seq_t val_a1 = cache[idx1];           /* a(a(i-2)) */
            idx_t idx2 = (idx_t)val_a1;           /* a(a(i-2))'s index */
            seq_t val_a2 = cache[idx2];           /* a(a(a(i-2))) */

            /* a(i - a(i-2)) */
            idx_t idx3 = i - (idx_t)val_n2;
            seq_t val_a3 = cache[idx3];

            cache[i] = val_a2 + val_a3;
        }
    }
    cached_max = n;
}

/* Public sequence function.  */
seq_t
a(idx_t n)
{
    if (n < 1)
        return 0;                 /* undefined for n < 1 */

    ensure_capacity(n);
    if (cached_max < n)
        populate_to(n);

    return cache[n];
}
