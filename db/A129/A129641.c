/*
 * A129641  Nonnegative values x of solutions (x, y) to the Diophantine
 *          equation x^2+(x+409)^2 = y^2.
 * https://oeis.org/A129641
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 10, u32 n <= 29, u64 n <= 30, u128 n <= 30,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A129641
    a(n) = 6*a(n-3) - a(n-6) + 818  for n > 6
    a(1)=0, a(2)=200, a(3)=611, a(4)=1227, a(5)=2291, a(6)=4620   */
#include <stdlib.h>

/* Persistent cache: cache[i] holds a(i) for i >= 1; cache[0] is unused. */
static seq_t *cache    = NULL;   /* array of seq_t */
static idx_t cache_cap = 0;      /* number of elements allocated (including index 0) */
static idx_t cache_len = 0;      /* highest index that currently holds a valid value */

/* Initialise cache with the first six values. */
static void initialise_cache(void)
{
    /* Allocate capacity for indices 0..6 (7 elements). */
    cache = (seq_t *)malloc(7 * sizeof(seq_t));
    cache_cap = 7;

    /* Set initial values (cache[0] is unused). */
    cache[0] = (seq_t)0;
    cache[1] = (seq_t)0;
    cache[2] = (seq_t)200;
    cache[3] = (seq_t)611;
    cache[4] = (seq_t)1227;
    cache[5] = (seq_t)2291;
    cache[6] = (seq_t)4620;

    cache_len = 6;
}

/* Grow cache to at least (new_cap) elements. */
static void ensure_capacity(idx_t new_cap)
{
    if (new_cap <= cache_cap)
        return;
    /* Allocate at least new_cap elements. */
    seq_t *tmp = (seq_t *)realloc(cache, (size_t)new_cap * sizeof(seq_t));
    cache = tmp;
    cache_cap = new_cap;
}

seq_t a(idx_t n)
{
    if (n < 1)
        return (seq_t)0;   /* the sequence is defined for n >= 1 */

    /* Lazy initialisation of the cache. */
    if (cache == NULL)
        initialise_cache();

    /* If the requested term is already known, return it directly. */
    if (n <= cache_len)
        return cache[n];

    /* Ensure the cache has enough space. */
    /* Add a small over‑provision to reduce frequent reallocations. */
    idx_t needed = n + 1;                /* indices start at 0 */
    if (needed > cache_cap)
    {
        idx_t new_cap = cache_cap * 2;
        if (new_cap < needed)
            new_cap = needed;
        ensure_capacity(new_cap);
    }

    /* Compute missing terms up to n. */
    for (idx_t i = cache_len + 1; i <= n; ++i)
    {
        /* The recurrence only applies for i > 6, but the loop is safe
           because the cache already contains indices 1..6. */
        seq_t term = 6 * cache[i - 3] - cache[i - 6] + (seq_t)818;
        cache[i] = term;
    }
    cache_len = n;

    return cache[n];
}
