/*
 * A123362  a(0) = 1, a(1) = 1, a(n) = 6*a(n-1) + 5*a(n-2) for n > 1.
 * https://oeis.org/A123362
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 12, u64 n <= 24, u128 n <= 47,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(0)=1, a(1)=1, a(n)=6*a(n-1)+5*a(n-2) */
#include <stdlib.h>
#include <stddef.h>

/* static cache for memoisation */
static seq_t *cache = NULL;
static size_t cache_size = 0;
static size_t cache_cap  = 0;

/* grow the cache to at least 'needed' entries */
static void _grow_cache(size_t needed)
{
    if (cache_cap >= needed)
        return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 8;
    while (new_cap < needed)
        new_cap *= 2;
    seq_t *new_cache = (seq_t*)malloc(new_cap * sizeof(seq_t));
    if (!new_cache)
        exit(1);                /* cannot allocate: terminate */
    if (cache) {
        for (size_t i = 0; i < cache_size; ++i)
            new_cache[i] = cache[i];
        free(cache);
    }
    cache = new_cache;
    cache_cap = new_cap;
}

/* public function required by the problem statement */
seq_t a(idx_t n)
{
    /* handle base cases */
    if (n == 0)
        return (seq_t)1;
    if (n == 1)
        return (seq_t)1;

    /* make sure the cache can hold up to n */
    size_t needed = (size_t)n + 1;
    _grow_cache(needed);

    /* initialise cache if not already done */
    if (cache_size == 0) {
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)1;
        cache_size = 2;
    }

    /* compute missing values up to n */
    for (size_t i = cache_size; i <= (size_t)n; ++i) {
        seq_t prev1 = cache[i - 1];
        seq_t prev2 = cache[i - 2];
        cache[i] = (seq_t)6 * prev1 + (seq_t)5 * prev2;
    }
    cache_size = needed;

    return cache[(size_t)n];
}
