/*
 * A214997  Power ceiling-floor sequence of 2+sqrt(2).
 * https://oeis.org/A214997
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3*a(n-1) + 2*a(n-2) - 2*a(n-3) Initial values: a(0)=4,
 *            a(1)=13, a(2)=45
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 16, u64 n <= 35, u128 n <= 71,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Cached values of the sequence. */
static seq_t *seq_cache = NULL;
static size_t cache_len = 0;   /* Number of terms computed so far. */
static size_t cache_cap = 0;   /* Allocated capacity. */

/* Ensure the cache has space for at least n+1 terms. */
static void grow_cache(idx_t n)
{
    size_t needed = (size_t)n + 1;
    if (cache_cap < needed)
    {
        /* Allocate or reallocate.  Use realloc for growth. */
        seq_t *new_mem = (seq_t *)realloc(seq_cache, needed * sizeof(seq_t));
        if (!new_mem) abort();          /* Allocation failure → abort. */
        seq_cache = new_mem;
        cache_cap = needed;
    }
}

/* Compute the sequence value a(n). */
seq_t a(idx_t n)
{
    /* Ensure cache is initialized. */
    if (cache_len == 0)
    {
        /* Allocate at least three slots and set the initial terms. */
        cache_cap = 3;
        seq_cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!seq_cache) abort();
        seq_cache[0] = (seq_t)4;
        seq_cache[1] = (seq_t)13;
        seq_cache[2] = (seq_t)45;
        cache_len = 3;
    }

    /* Grow cache if the requested index is beyond current capacity. */
    if ((size_t)n >= cache_cap)
        grow_cache(n);

    /* Compute missing terms up to n. */
    while (cache_len <= (size_t)n)
    {
        /* Recurrence: a(n) = 3*a(n-1) + 2*a(n-2) - 2*a(n-3) */
        seq_t a1 = seq_cache[cache_len - 1];
        seq_t a2 = seq_cache[cache_len - 2];
        seq_t a3 = seq_cache[cache_len - 3];
        seq_t val = 3 * a1 + 2 * a2 - 2 * a3;
        seq_cache[cache_len] = val;
        ++cache_len;
    }

    return seq_cache[(size_t)n];
}
