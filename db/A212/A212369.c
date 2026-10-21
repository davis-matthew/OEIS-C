/*
 * A212369  Number of Dyck n-paths all of whose ascents and descents have
 *          lengths equal to 1 (mod 10).
 * https://oeis.org/A212369
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 25, u16 n <= 42, u32 n <= 74, u64 n <= 138,
 *            u128 n <= 264, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A212369: Number of Dyck n-paths all of whose ascents and descents
   have lengths equal to 1 (mod 10).  Recurrence:
   a(0) = 1;
   a(n) = a(n-1) + Σ_{k=1}^{n-10} a(k) * a(n-10-k)   for n > 0.
   Initial values: a(0)=...=a(10)=1. */

#include <stdlib.h>

static seq_t *cache = NULL;      /* cached values a(0)..a(k-1) */
static idx_t cache_size = 0;     /* number of cached elements (cache[-1] is last index) */

/* Computes a(n) for any n >= 0, memoizing results in a static cache. */
seq_t a(idx_t n)
{
    /* Grow the cache as needed. */
    if (n + 1 > cache_size) {
        seq_t *new_cache = (seq_t *)realloc(cache, (n + 1) * sizeof(seq_t));
        if (!new_cache) abort();   /* out of memory */
        cache = new_cache;
        /* Compute new entries, starting from the current cache_size. */
        for (idx_t i = cache_size; i <= n; ++i) {
            if (i == 0) {
                cache[i] = 1;
            } else {
                seq_t val = cache[i - 1];        /* a(i-1) */
                if (i >= 10) {
                    for (idx_t k = 1; k <= i - 10; ++k) {
                        val += cache[k] * cache[i - 10 - k];
                    }
                }
                cache[i] = val;
            }
        }
        cache_size = n + 1;
    }
    return cache[n];
}
