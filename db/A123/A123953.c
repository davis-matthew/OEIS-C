/*
 * A123953  a(0)=1. a(n) = number of terms among terms a(0) through a(n-1) of
 *          the sequence which are coprime to n(n+1)/2.
 * https://oeis.org/A123953
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of previous terms a(i), 0 ≤ i < n, that are
 *            coprime to n(n+1)/2 with a(0) = 1
 * Range:     u8 n <= 22, u16 n <= 77, u32 n <= 77, u64 n <= 77, u128 n <= 77,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/gcd.h"   /* provides seq_t gcd(seq_t, seq_t) */

seq_t a(idx_t n)
{
    /* static cache of already computed values */
    static seq_t *cache = NULL;   /* array of a(i) values */
    static size_t  size  = 0;     /* number of filled entries in cache */
    static size_t  cap   = 0;     /* allocated capacity of cache */

    size_t target = (size_t)n;

    /* grow the cache if necessary */
    if (size <= target) {
        size_t needed = target + 1;
        if (cap < needed) {
            size_t new_cap = needed * 2 ? needed * 2 : needed; /* avoid 0 cap */
            seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            if (!tmp) abort();   /* out of memory */
            cache = tmp;
            cap   = new_cap;
        }

        /* compute missing terms from current size up to n */
        for (size_t idx = size; idx <= target; ++idx) {
            if (idx == 0) {
                /* base case a(0) = 1 */
                cache[0] = (seq_t)1;
                continue;
            }
            seq_t tri   = (seq_t)idx * ((seq_t)idx + 1) / 2;
            seq_t count = (seq_t)0;
            for (size_t j = 0; j < idx; ++j)
                if (gcd(cache[j], tri) == (seq_t)1)
                    ++count;
            cache[idx] = count;
        }
        size = target + 1;
    }

    return cache[target];
}
