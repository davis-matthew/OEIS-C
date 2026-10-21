/*
 * A236294  a(n) = max( a(n-1) + a(n-3), 2*a(n-2) ) - a(n-4), with a(0)=1,
 *          a(1)=1, a(2)=2, a(3)=3.
 * https://oeis.org/A236294
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = max( a(n-1)+a(n-3), 2*a(n-2) ) - a(n-4) with initial
 *            values a(0)=1, a(1)=1, a(2)=2, a(3)=3. The function computes the
 *            term for any n ≥ 0 in any call order
 * Range:     u8 n <= 60, u16 n <= 1020, u32 n <= 2500, u64 n <= 2500,
 *            u128 n <= 2500, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Static cache of already computed values */
static seq_t *cache = NULL;      /* array of a(i) */
static idx_t  cache_len = 0;     /* number of values stored */
static size_t cache_cap = 0;     /* current capacity of the array */

seq_t a(idx_t n)
{
    /* Base cases known directly. */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)2;
    if (n == 3) return (seq_t)3;

    /* If we already have the value, return it immediately. */
    if (cache && cache_len > n)
        return cache[n];

    /* Initialise cache if this is the first usage or if capacity is insufficient. */
    if (!cache || cache_len == 0) {
        /* Allocate initial space. 8 is arbitrary but gives room for growth. */
        cache_cap = 8;
        cache = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
        if (!cache) exit(EXIT_FAILURE); /* unexpected allocation failure */

        /* Store the first four known terms. */
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)1;
        cache[2] = (seq_t)2;
        cache[3] = (seq_t)3;
        cache_len = 4;
    }

    /* Grow cache as needed to reach the required index n. */
    for (idx_t i = cache_len; i <= n; ++i) {
        if (i == cache_cap) {
            /* Double the capacity. */
            size_t new_cap = cache_cap * 2;
            seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            if (!tmp) exit(EXIT_FAILURE);
            cache = tmp;
            cache_cap = new_cap;
        }

        /* Compute a(i) using the recurrence. */
        seq_t t1 = cache[i-1] + cache[i-3];          /* a(i-1)+a(i-3) */
        seq_t t2 = (seq_t)2 * cache[i-2];            /* 2*a(i-2)   */
        seq_t maxv = (t1 > t2) ? t1 : t2;            /* max( t1, t2 ) */
        cache[i] = maxv - cache[i-4];                /* final value */
    }

    cache_len = n + 1;
    return cache[n];
}
