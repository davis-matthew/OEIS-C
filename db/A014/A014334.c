/*
 * A014334  Exponential convolution of Fibonacci numbers with themselves.
 * https://oeis.org/A014334
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3*a(n-1) + 2*a(n-2) - 4*a(n-3), a(0)=0, a(1)=0, a(2)=2
 * Range:     u8 n <= 6, u16 n <= 10, u32 n <= 20, u64 n <= 39, u128 n <= 76,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Cache for computed terms. */
static seq_t *cache = NULL;      /* array holding computed terms              */
static idx_t   cap   = 0;        /* current capacity of the cache            */
static idx_t   last  = 0;        /* greatest index that has been computed    */

/* Grow the cache so that it can store indices up to n (inclusive). */
static void ensure_cache(idx_t n)
{
    if (n < cap)
        return;                           /* already enough space */

    /* Determine new size: double until it exceeds n. */
    idx_t new_cap = cap ? cap : 1;
    while (new_cap <= n)
        new_cap <<= 1;

    /* Allocate new array and copy old data. */
    seq_t *new_cache = (seq_t *)malloc(new_cap * sizeof(seq_t));
    if (!new_cache)
        return; /* allocation failed; caller will handle */

    for (idx_t i = 0; i < cap; ++i)
        new_cache[i] = cache[i];
    for (idx_t i = cap; i < new_cap; ++i)
        new_cache[i] = 0;                   /* initialise new slots */

    /* Free old cache if it existed. */
    if (cache)
        free(cache);

    cache = new_cache;
    cap   = new_cap;
}

/* Compute the nth term of A014334. */
seq_t a(idx_t n)
{
    /* Initialise cache on first call. */
    if (!cache) {
        ensure_cache(2);                   /* need at least indices 0..2 */
        cache[0] = 0;
        cache[1] = 0;
        cache[2] = 2;
        last = 2;
    }

    /* Return already computed value. */
    if (n <= last)
        return cache[n];

    /* Grow cache if necessary. */
    ensure_cache(n);

    /* Fill in missing values by recurrence. */
    for (idx_t i = last + 1; i <= n; ++i) {
        seq_t term = 3 * cache[i-1] + 2 * cache[i-2];
        term -= 4 * cache[i-3];
        cache[i] = term;
    }
    last = n;

    return cache[n];
}
