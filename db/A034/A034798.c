/*
 * A034798  Value of n considered as a game.
 * https://oeis.org/A034798
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = mex{ a(i) | the i-th bit of n is 1 }
 * Range:     u8 n <= 99, u16 n <= 99, u32 n <= 99, u64 n <= 99, u128 n <= 99,
 *            big any n
 * Helpers:   ensure_cache
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>               /* for size_t, NULL */
#include "../utils/ensure_cache.h"

seq_t a(idx_t n)
{
    /* static cache for previously computed values */
    static seq_t *cache   = NULL;   /* array of a(i) values */
    static idx_t last_done = 0;     /* highest index computed so far */
    static size_t cap = 0;          /* current allocated capacity */
    static bool initialized = false;

    /* initialise base case once */
    if (!initialized) {
        /* make sure we have room for a(0) */
        ensure_cache(1, &cache, &cap);
        cache[0] = (seq_t)0;
        last_done = 0;
        initialized = true;
    }

    /* extend cache if necessary */
    if (n + 1 > cap) {
        ensure_cache((size_t)n + 1, &cache, &cap);
    }

    /* compute missing values up to n */
    for (idx_t i = last_done + 1; i <= n; ++i) {
        seq_t candidate = 0;
        /* find mex of a(j) for each set bit j in i */
        while (1) {
            bool clash = false;
            idx_t bits = i;
            idx_t pos  = 0;
            while (bits) {
                if (bits & 1ULL) {
                    if (cache[pos] == candidate) {
                        clash = true;
                        break;
                    }
                }
                bits >>= 1;
                pos++;
            }
            if (!clash) break;
            candidate++;
        }
        cache[i] = candidate;
    }

    last_done = n;
    return cache[n];
}
