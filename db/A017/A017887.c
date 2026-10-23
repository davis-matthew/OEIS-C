/*
 * A017887  Expansion of 1/(1 - x^10 - x^11).
 * https://oeis.org/A017887
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-10) + a(n-11) for n >= 12 Initial values: a(0)=1,
 *            a(1..9)=0, a(10)=1, a(11)=1 Computes the number of ordered
 *            partitions of n into parts 10 and 11
 * Range:     u8 n <= 113, u16 n <= 197, u32 n <= 366, u64 n <= 708,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

/* Static cache of computed values with memoization */
static seq_t *cache = NULL;          /* array holding a(0)...a(k) */
static idx_t  cached_max = (idx_t)-1;/* largest index stored; (idx_t)-1 means cache empty */

/* Ensure the cache contains entries up to n */
static void ensure_cache(idx_t n)
{
    /* If cache already has all needed values, nothing to do. */
    if (cache != NULL && cached_max >= n)
        return;

    /* Allocate initial cache if necessary. */
    if (cache == NULL) {
        size_t init_size = 12;          /* indices 0..11 */
        cache = (seq_t *)malloc(init_size * sizeof(seq_t));
        if (!cache)
            return;                    /* allocation failure; leave cache NULL */

        /* initialise base cases */
        cache[0] = (seq_t)1;
        for (idx_t i = 1; i <= 9; ++i)
            cache[i] = (seq_t)0;
        cache[10] = (seq_t)1;
        cache[11] = (seq_t)1;
        cached_max = 11;
    }

    /* Grow the cache if n exceeds the current capacity. */
    idx_t needed = n;
    if (needed <= cached_max)          /* already ready after allocation */
        return;

    size_t new_size = (size_t)(needed + 1);
    size_t curr_size = (size_t)(cached_max + 1);

    /* Optional doubling strategy for growth efficiency. */
    while (curr_size < new_size)
        curr_size <<= 1;

    seq_t *tmp = (seq_t *)realloc(cache, curr_size * sizeof(seq_t));
    if (!tmp)
        return;                        /* realloc failed; leave cache unchanged */
    cache = tmp;

    /* Compute missing values using the recurrence. */
    for (idx_t i = cached_max + 1; i <= needed; ++i)
        cache[i] = cache[i - 10] + cache[i - 11];

    cached_max = needed;
}

seq_t a(idx_t n)
{
    ensure_cache(n);
    /* On allocation failure cache may be NULL; return 0 in that case. */
    if (!cache)
        return (seq_t)0;
    return cache[n];
}
