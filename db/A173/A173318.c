/*
 * A173318  Partial sums of A005811.
 * https://oeis.org/A173318
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = sum_{i=0..n} (popcount(i ^ (i >> 1)))
 * Range:     u8 n <= 78, u16 n <= 8191, u32 n <= 8191, u64 n <= 8191,
 *            u128 n <= 8191, big any n
 * Validated: standard, pass, 78 terms through n = 7939 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  a(0) = 0  */
#include <stdlib.h>
#include <stdint.h>
#include "../utils/mem.h"

/* Count the runs of bits in the binary representation of n.
   For n = 0 the result is 0. */
static inline seq_t runs_bits(idx_t n)
{
    if (n == 0) return (seq_t)0;
    uint64_t x = (uint64_t)n;
    return (seq_t)__builtin_popcountll(x ^ (x >> 1));
}

seq_t a(idx_t n)
{
    /* static cache of partial sums */
    static seq_t *cache = NULL;      /* cache[i] = a(i) */
    static idx_t cache_len = 0;      /* number of stored elements (last index + 1) */

    /* initialize cache with a(0) */
    if (cache == NULL) {
        cache_len = 1;
        cache = mem_new(seq_t, 1);
        cache[0] = (seq_t)0;
    }

    /* extend cache up to n if necessary */
    if (n >= cache_len) {
        size_t new_len = (size_t)n + 1;      /* new number of elements */
        seq_t *new_cache = mem_resize(seq_t, cache, new_len);
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
