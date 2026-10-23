/*
 * A130121  a(1)=1. a(n) = number of earlier terms of the sequence which
 *          divide n(n+1)/2.
 * https://oeis.org/A130121
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of earlier terms of the sequence which divide
 *            n*(n+1)/2 a(1) is defined to be 1. The sequence is memoized; any
 *            call order is allowed
 * Range:     u8 n <= 22, u16 n <= 542, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   grow_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>
#include "../utils/grow_seq.h"   /* for grow_seq(seq_t **, size_t *, size_t) */

static seq_t *cache = NULL;          /* stores a(1)..a(m)  (0‑based for a(1)) */
static size_t cache_cap = 0;         /* capacity of cache */
static idx_t  cache_len  = 0;        /* number of terms already computed */

seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;  /* undefined for n<1, harmless */

    /* Expand the cache until it contains a(n) */
    if (cache_len < n) {
        size_t needed = (size_t)n;           /* we need space for n terms */
        if (needed > cache_cap) grow_seq(&cache, &cache_cap, needed);

        for (idx_t idx = cache_len + 1; idx <= n; ++idx) {
            /* triangular number T = idx*(idx+1)/2 computed in seq_t */
            seq_t T = (seq_t)idx * ((seq_t)idx + 1) / 2;
            seq_t cnt = 0;

            if (idx == 1) {
                cnt = 1;                     /* a(1) = 1 by definition */
            } else {
                /* count how many of the previous terms divide T */
                for (idx_t j = 1; j < idx; ++j)
                    if (T % cache[j - 1] == 0) ++cnt;
            }
            cache[idx - 1] = cnt;
        }
        cache_len = n;
    }
    return cache[n - 1];
}
