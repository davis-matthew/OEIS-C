/*
 * A184549  Super-birthdays (falling on the same weekday), version 1 (birth
 *          within the year following a February 29).
 * https://oeis.org/A184549
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1)+a(n-4)-a(n-5); Initial values: a(0)=0, a(1)=6,
 *            a(2)=17, a(3)=23, a(4)=28
 * Range:     u8 n <= 36, u16 n <= 9361, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>


static seq_t *cache = NULL;          /* dynamic array of computed terms */
static idx_t  cache_size = 0;        /* number of elements currently in cache (might be < n+1) */

/* Ensure that indices 0…n are computed and stored in cache */
static void ensure_terms(idx_t n)
{
    if (n + 1 <= cache_size)          /* already available */
        return;

    /* Allocate or grow the cache to hold indices 0…n (inclusive) */
    seq_t *new_cache = (seq_t*)realloc(cache, (n + 1) * sizeof(seq_t));
    if (!new_cache)                    /* allocation failed – leave old cache untouched */
        return;

    /* Overwrite the first five values with the known constants.
       It is safe to write them again even if they already had the same values. */
    if (n >= 0) new_cache[0] = (seq_t)0;
    if (n >= 1) new_cache[1] = (seq_t)6;
    if (n >= 2) new_cache[2] = (seq_t)17;
    if (n >= 3) new_cache[3] = (seq_t)23;
    if (n >= 4) new_cache[4] = (seq_t)28;

    /* Compute new terms using the recurrence. */
    idx_t start = cache_size > 5 ? cache_size : 5;   /* first index that needs computation */
    for (idx_t i = start; i <= n; ++i)
    {
        new_cache[i] = new_cache[i-1] + new_cache[i-4] - new_cache[i-5];
    }

    cache = new_cache;
    cache_size = n + 1;                    /* now indices 0…n are available */
}

/* Public sequence function */
seq_t a(idx_t n)
{
    ensure_terms(n);
    return (cache && n < cache_size) ? cache[n] : (seq_t)0;
}
