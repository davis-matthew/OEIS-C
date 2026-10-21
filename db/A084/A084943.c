/*
 * A084943  Decagorials: n-th polygorial for k=10.
 * https://oeis.org/A084943
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = a(n-1) * n * (4*n-3), with a(0) = 1
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 6, u64 n <= 10, u128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

/* Local cache for memoised values */
static seq_t *cache   = NULL;   /* a(0), a(1), ... */
static idx_t  cache_len  = 0;   /* next index to compute + 1 */
static idx_t  cache_cap  = 0;   /* allocated capacity */

/* Ensure the cache can hold indices up to n (inclusive) */
static void grow_cache(idx_t n)
{
    if (cache_cap <= n) {
        idx_t new_cap = cache_cap ? cache_cap * 2 : 16;
        while (new_cap <= n) new_cap *= 2;
        /* cast is allowed and sufficient even when seq_t is an
           arbitrary‑precision integer class (trivial type) */
        cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        cache_cap = new_cap;
    }
}

/* Compute a(n) for the OEIS sequence A084943 (decagorials) */
seq_t a(idx_t n)
{
    /* initialise the cache on first call */
    if (cache_len == 0) {
        grow_cache(0);
        cache[0] = (seq_t)1;   /* a(0) = 1 */
        cache_len = 1;
    }

    /* extend cached values up to n */
    if (cache_len <= n) {
        grow_cache(n);
        for (idx_t i = cache_len; i <= n; ++i) {
            seq_t prev   = cache[i-1];
            seq_t mult   = (seq_t)i;                      /* n */
            seq_t factor = (seq_t)( (4 * (int64_t)i) - 3 );/* 4n-3 */
            cache[i] = prev * mult * factor;
        }
        cache_len = n + 1;
    }

    return cache[n];
}
