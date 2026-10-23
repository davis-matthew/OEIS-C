/*
 * A137412  a(1)=0. If a(m) is odd, then a(2^(m-1)+k) = a(k)-1, for all k
 *          where 1<=k<=2^(m-1). If a(m) is even, then a(2^(m-1)+k) = a(k)+1,
 *          for all k where 1<=k<=2^(m-1).
 * https://oeis.org/A137412
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, sign
 * Formula:   a(1)=0; for m>1, if a(m) is odd then a(2^(m-1)+k)=a(k)-1, else
 *            a(2^(m-1)+k)=a(k)+1 (k=1..2^(m-1))
 * Range:     i8 n <= 114, i16 n <= 114, i32 n <= 114, i64 n <= 114,
 *            i128 n <= 114, big any n
 * Helpers:   block_info
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include "../utils/block_info.h"

seq_t a(idx_t n)
{
    if (n < 1)
        return 0; /* undefined for n<1, but return 0 */

    static seq_t *cache = NULL;
    static idx_t cache_size = 0;
    static idx_t max_computed = 0;

    /* Allocate or grow cache to hold up to n */
    if (n + 1 > cache_size) {
        idx_t old_size = cache_size;
        cache_size = n + 1;                    /* +1 so index n is valid */
        cache = (seq_t *)realloc(cache, cache_size * sizeof(seq_t));
        if (old_size < cache_size)
            memset(cache + old_size, 0, (cache_size - old_size) * sizeof(seq_t));
    }

    /* Initialize base case */
    if (max_computed < 1) {
        cache[1] = 0;
        max_computed = 1;
    }

    /* Compute missing terms up to n */
    for (idx_t i = max_computed + 1; i <= n; ++i) {
        idx_t start, m;
        block_info(i, &start, &m);   /* start = 2^(m-1), m is block index */
        idx_t k = i - start;         /* 1 <= k <= start */
        seq_t val = cache[k];
        if (cache[m] & 1)            /* a(m) odd */
            cache[i] = val - 1;
        else                         /* a(m) even */
            cache[i] = val + 1;
    }

    max_computed = (max_computed > n) ? max_computed : n;
    return cache[n];
}
