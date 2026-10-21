/*
 * A202564  Indices of pentagonal numbers which are also decagonal.
 * https://oeis.org/A202564
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 98*a(n-1) - a(n-2) - 16, a(1)=1, a(2)=91
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 5, u64 n <= 10, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This function computes the OEIS sequence A202564 using memoised
 * recurrence. It works for any seq_t type defined by the build
 * (unsigned 8/16/32/64-bit, unsigned __int128, or arbitrary precision)
 * and any idx_t type (uint64_t). */

#include <stddef.h>
#include <stdlib.h>

/* Static cache: cache[i] holds a(i), i >= 1. Index 0 is unused. */
static seq_t *cache = NULL;
static size_t cache_cap = 0;   /* current allocated capacity (including index 0) */
static idx_t  cache_len = 0;   /* largest n that has been computed */

seq_t a(idx_t n)
{
    /* Undefined for n < 1 – return 0 (sequence only defined for n >= 1). */
    if (n < (idx_t)1)
        return (seq_t)0;

    /* Need space for indices 1..n (index 0 is unused). */
    size_t need = (size_t)n + 1U;

    /* Allocate or grow the cache. */
    if (cache == NULL) {
        cache = (seq_t *)malloc(need * sizeof(seq_t));
        cache_cap = need;
    } else if (need > cache_cap) {
        seq_t *tmp = (seq_t *)realloc(cache, need * sizeof(seq_t));
        if (tmp == NULL) {
            /* Allocation failed – aborting computation. */
            abort();
        }
        cache = tmp;
        cache_cap = need;
    }

    /* Compute missing values up to n. */
    for (idx_t i = cache_len + (idx_t)1; i <= n; ++i) {
        if (i == (idx_t)1) {
            cache[1] = (seq_t)1;
        } else if (i == (idx_t)2) {
            cache[2] = (seq_t)91;
        } else {
            /* recurrence: a(n) = 98*a(n-1) - a(n-2) - 16 */
            seq_t val = (seq_t)98 * cache[i - 1];
            val -= cache[i - 2];
            val -= (seq_t)16;
            cache[i] = val;
        }
    }

    cache_len = n;
    return cache[n];
}
