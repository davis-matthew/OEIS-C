/*
 * A100335  An inverse Catalan transform of J(2n).
 * https://oeis.org/A100335
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 5*a(n-1) - 9*a(n-2) + 8*a(n-3) - 4*a(n-4), with a(0)=0,
 *            a(1)=1, a(2)=4, a(3)=11
 * Range:     u8 n <= 6, u16 n <= 13, u32 n <= 28, u64 n <= 59, u128 n <= 122,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;          /* memoisation table */
static idx_t cache_size  = 0;        /* number of elements stored (valid indices 0..cache_size-1) */
static idx_t cache_cap   = 0;        /* allocated capacity of 'cache' */

/* Helper to grow the cache to at least 'needed' elements */
static void ensure_capacity(idx_t needed)
{
    if (cache_cap >= needed)
        return;
    idx_t new_cap = cache_cap ? cache_cap * 2 : 4;
    while (new_cap < needed)
        new_cap *= 2;
    /* realloc keeps existing data; cast needed for C++ */
    cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    cache_cap = new_cap;
}

seq_t a(idx_t n)
{
    /* initialise cache with first four values on first call */
    if (cache_size == 0) {
        ensure_capacity(4);
        cache[0] = (seq_t)0;
        cache[1] = (seq_t)1;
        cache[2] = (seq_t)4;
        cache[3] = (seq_t)11;
        cache_size = 4;
    }

    if (n < cache_size)
        return cache[n];

    ensure_capacity(n + 1);

    for (idx_t i = cache_size; i <= n; ++i) {
        /* recurrence: a(i) = 5*a(i-1) - 9*a(i-2) + 8*a(i-3) - 4*a(i-4) */
        seq_t term = (seq_t)5 * cache[i - 1]
                   - (seq_t)9 * cache[i - 2]
                   + (seq_t)8 * cache[i - 3]
                   - (seq_t)4 * cache[i - 4];
        cache[i] = term;
    }
    cache_size = n + 1;
    return cache[n];
}
