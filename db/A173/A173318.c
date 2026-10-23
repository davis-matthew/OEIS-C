/*
 * A173318  Partial sums of A005811.
 * https://oeis.org/A173318
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = sum_{i=0..n} (popcount(i ^ (i >> 1)))
 * Range:     u8 n <= 78, u16 n <= 8191, u32 n <= 8191, u64 n <= 8191,
 *            u128 n <= 8191, big any n
 * Helpers:   runs_bits
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  a(0) = 0  */
#include <stdlib.h>
#include <stdint.h>
#include "../utils/runs_bits.h"

seq_t a(idx_t n)
{
    /* static cache of partial sums */
    static seq_t *cache = NULL;      /* cache[i] = a(i) */
    static idx_t cache_len = 0;      /* number of stored elements (last index + 1) */

    /* initialize cache with a(0) */
    if (cache == NULL) {
        cache_len = 1;
        cache = (seq_t *)malloc(sizeof(seq_t));
        cache[0] = (seq_t)0;
    }

    /* extend cache up to n if necessary */
    if (n >= cache_len) {
        size_t new_len = (size_t)n + 1;      /* new number of elements */
        seq_t *new_cache = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
        if (new_cache == NULL) {
            /* out‑of‑memory: abort or handle as appropriate */
            return (seq_t)0;
        }
        cache = new_cache;
        for (idx_t i = cache_len; i <= n; ++i) {
            seq_t r = runs_bits(i);
            cache[i] = cache[i-1] + r;
        }
        cache_len = new_len;
    }

    return cache[n];
}
