/*
 * A232317  Number of (1+1)X(n+1) 0..1 arrays with every element equal to some
 *          horizontal or antidiagonal neighbor, with top left element zero.
 * https://oeis.org/A232317
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 3*a(n-1) + a(n-2) - 2*a(n-4) for n>5, a(1)=2, a(2)=5,
 *            a(3)=16, a(4)=52, a(5)=169
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 19, u64 n <= 38, u128 n <= 75,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The code uses a simple memoised recurrence; it works for any
   integer type seq_t (including arbitrary‑precision) and any
   unsigned index type idx_t. */

#include <stdlib.h>
#include <stddef.h>

/* Static cache of already computed terms (index 0 unused). */
static seq_t *cache      = NULL;
static size_t cache_cap  = 0;   /* allocated size */
static idx_t  cache_max  = 0;   /* largest n that has been computed */

/* Resize the cache to at least n+1 elements, preserving existing values. */
static void ensure_cache(idx_t n)
{
    if ((size_t)(n + 1) <= cache_cap)
        return;

    size_t new_cap = (size_t)(n + 1);
    seq_t *tmp = (seq_t *)malloc(new_cap * sizeof(seq_t));
    if (!tmp)            /* allocation failed */
        exit(1);

    /* copy old values (if any) */
    for (size_t i = 0; i < cache_cap; ++i)
        tmp[i] = cache[i];

    if (cache)
        free(cache);

    cache     = tmp;
    cache_cap = new_cap;
}

/* Initialise the cache with the first five terms of the sequence. */
static void init_cache(void)
{
    ensure_cache(5);
    cache[1] = (seq_t)2;
    cache[2] = (seq_t)5;
    cache[3] = (seq_t)16;
    cache[4] = (seq_t)52;
    cache[5] = (seq_t)169;
    cache_max = 5;
}

/* Public sequence function. */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;          /* the offset is 1; n=0 is unused */

    /* Initialise once */
    if (cache == NULL)
        init_cache();

    /* Compute terms up to n if necessary */
    if (n > cache_max) {
        ensure_cache(n); /* ensure capacity */
        for (idx_t i = cache_max + 1; i <= n; ++i) {
            if (i <= 5) {
                /* base values already stored; nothing to do */
                continue;
            }
            /* recurrence: a(i) = 3*a(i-1) + a(i-2) - 2*a(i-4) */
            cache[i] = 3 * cache[i - 1] + cache[i - 2] - 2 * cache[i - 4];
        }
        cache_max = n;
    }

    return cache[n];
}
