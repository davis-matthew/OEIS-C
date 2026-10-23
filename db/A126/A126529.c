/*
 * A126529  Number of base 8 n-digit numbers with adjacent digits differing by
 *          five or less.
 * https://oeis.org/A126529
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = 7*a(n-1) + 3*a(n-2) - 4*a(n-3) (n > 2) Initial values:
 *            a(0)=1, a(1)=8, a(2)=58
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 11, u64 n <= 19, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

static seq_t *cache = NULL;          /* Cached sequence values */
static size_t cache_cap = 0;         /* Allocated capacity of cache   */
static size_t max_computed = 0;      /* Largest index already computed */

/* Ensure the cache is large enough to hold indices up to n.
   Reallocate with exponential growth to keep O(n) allocations. */
static void
ensure_cache(idx_t n)
{
    size_t needed = (size_t)(n + 1);
    if (cache_cap >= needed)
        return;
    /* Grow exponentially */
    size_t new_cap = cache_cap ? cache_cap * 2 : 1;
    while (new_cap < needed)
        new_cap *= 2;
    seq_t *new_cache = (seq_t *)malloc(new_cap * sizeof(seq_t));
    if (!new_cache)
        abort();   /* out of memory; terminate */
    /* Copy existing values */
    for (size_t i = 0; i < cache_cap; ++i)
        new_cache[i] = cache[i];
    /* Free old cache */
    free(cache);
    cache = new_cache;
    cache_cap = new_cap;
}

seq_t
a(idx_t n)
{
    /* Handle the first three initial terms directly */
    if (n == 0)
        return (seq_t)1;
    if (n == 1)
        return (seq_t)8;
    if (n == 2)
        return (seq_t)58;

    /* Ensure the cache can hold up to index n */
    ensure_cache(n);

    /* Initialize base values if not already done */
    if (max_computed < 2) {
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)8;
        cache[2] = (seq_t)58;
        max_computed = 2;
    }

    /* Compute values up to n using the recurrence */
    for (size_t i = max_computed + 1; i <= (size_t)n; ++i) {
        cache[i] = (seq_t)7 * cache[i - 1]
                 + (seq_t)3 * cache[i - 2]
                 - (seq_t)4 * cache[i - 3];
    }
    max_computed = (size_t)n;

    return cache[(size_t)n];
}
