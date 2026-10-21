/*
 * A097741  Pell equation solutions (10*a(n))^2 - 101*b(n)^2 = -1 with b(n) =
 *          A097742(n), n >= 0.
 * https://oeis.org/A097741
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 0, u16 n <= 1, u32 n <= 3, u64 n <= 7, u128 n <= 14,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: OEIS A097741
   Recurrence: a(0) = 1, a(1) = 403
                a(n) = 402*a(n-1) - a(n-2)  for n >= 2
 */
#include <stdlib.h>

static seq_t *cache = NULL;   /* memoisation table */
static idx_t cache_size = 0;   /* number of entries stored */
static idx_t cache_cap  = 0;   /* allocated capacity */

/* Ensure the cache can hold at least 'need' entries */
static void ensure_cache_cap(idx_t need)
{
    if (need < cache_cap)
        return;
    idx_t new_cap = cache_cap ? cache_cap : 4;
    while (new_cap <= need)
        new_cap <<= 1;          /* double until big enough */
    seq_t *new_ptr = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!new_ptr) abort();    /* allocation failed */
    cache = new_ptr;
    cache_cap = new_cap;
}

seq_t a(idx_t n)
{
    /* initialize cache on first call */
    if (cache_size == 0) {
        ensure_cache_cap(1);
        cache[0] = (seq_t)1;
        if (n == 0)
            return cache[0];
        ensure_cache_cap(2);
        cache[1] = (seq_t)403;
        cache_size = 2;
    }

    if (n < cache_size)
        return cache[n];

    /* grow cache to include n */
    ensure_cache_cap(n);
    for (idx_t i = cache_size; i <= n; ++i) {
        seq_t prev1 = cache[i - 1];
        seq_t prev2 = cache[i - 2];
        cache[i] = 402 * prev1 - prev2;
    }
    cache_size = n + 1;
    return cache[n];
}
