/*
 * A145329  Partial sums of A051442, starting at n=1.
 * https://oeis.org/A145329
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sum_{k=1..n} (k^(k+1) + (k+1)^k)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 8, u64 n <= 15, u128 n <= 17,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/big_pow.h"

#ifndef FREESEQ
#define FREESEQ(x) free(x)
#endif

seq_t a(idx_t n)
{
    /* cache[0] holds a(0) = 0 */
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;   /* number of cached elements */

    if (n == 0) {
        return 0;   /* outside the offset, harmless */
    }

    /* initialise the cache on first use */
    if (!cache) {
        cache_size = 1;
        cache = (seq_t *)malloc(sizeof(seq_t) * cache_size);
        if (!cache) exit(1);
        cache[0] = 0;   /* a(0) = 0 */
    }

    /* extend cache if necessary */
    if (n >= cache_size) {
        idx_t new_size = n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, sizeof(seq_t) * new_size);
        if (!tmp) exit(1);
        cache = tmp;

        /* compute new terms */
        for (idx_t i = cache_size; i <= n; ++i) {
            seq_t term1 = big_pow((seq_t)i, (seq_t)(i + 1));
            seq_t term2 = big_pow((seq_t)(i + 1), (seq_t)i);
            cache[i] = cache[i - 1] + term1 + term2;
        }
        cache_size = new_size;
    }

    return cache[n];
}
