/*
 * A131328  Row sums of triangle A131327.
 * https://oeis.org/A131328
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1) + 2*a(n-2) - a(n-3) - a(n-4) (n>3) Initial values:
 *            a(0)=1, a(1)=4, a(2)=5, a(3)=12 Works for any idx_t = uint64_t
 *            and any seq_t (unsigned or arbitrary precision)
 * Range:     u8 n <= 9, u16 n <= 20, u32 n <= 43, u64 n <= 89, u128 n <= 182,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

/* Cache for already computed values */
static seq_t *cache = NULL;                  /* pointer to allocated array */
static size_t cache_len = 0;                 /* number of valid cached elements */
static size_t cache_cap = 0;                 /* total allocated capacity */

/* Grow the cache to at least new_cap elements */
static void grow_cache(size_t new_cap)
{
    if (new_cap <= cache_cap)
        return;

    size_t new_size = (cache_cap == 0) ? 4 : cache_cap * 2;
    while (new_size < new_cap)
        new_size *= 2;

    seq_t *p = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
    if (p == NULL) {
        /* Allocation failed – terminate with an error */
        abort();
    }
    cache = p;
    cache_cap = new_size;
}

/* Initialise the cache with the first four terms */
static void init_cache(void)
{
    if (cache_len == 0) {
        grow_cache(4);
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)4;
        cache[2] = (seq_t)5;
        cache[3] = (seq_t)12;
        cache_len = 4;
    }
}

/* Return the nth term of A131328 */
seq_t a(idx_t n)
{
    init_cache();

    if ((size_t)n < cache_len)        /* already computed */
        return cache[(size_t)n];

    /* Grow cache so it can store up to index n */
    grow_cache((size_t)n + 1);

    /* Compute missing entries up to n */
    for (size_t i = cache_len; i <= (size_t)n; ++i) {
        /* recurrence: a[i] = a[i-1] + 2*a[i-2] - a[i-3] - a[i-4] */
        seq_t v1 = cache[i - 1];
        seq_t v2 = cache[i - 2];
        seq_t v3 = cache[i - 3];
        seq_t v4 = cache[i - 4];

        /* avoid multiplication with potential type issues by adding twice */
        cache[i] = v1 + v2 + v2 - v3 - v4;
    }

    cache_len = (size_t)n + 1;
    return cache[(size_t)n];
}
