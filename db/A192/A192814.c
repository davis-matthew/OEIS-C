/*
 * A192814  Constant term in the reduction of the polynomial (2*x+1)^n by x^3
 *          -> x^2 + x + 1. See Comments.
 * https://oeis.org/A192814
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 5*a(n-1) - 3*a(n-2) + 7*a(n-3), a(0)=a(1)=a(2)=1
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 15, u64 n <= 30, u128 n <= 58,
 *            big any n
 * Helpers:   grow_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include "../utils/grow_seq.h"

seq_t a(idx_t n)
{
    /* static cache: entries a(0)..a(cache_len-1) stored in cache[0..] */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;     /* allocated capacity */
    static size_t cache_len = 0;     /* number of terms actually computed */

    /* convert idx_t to size_t for array indexing */
    size_t sn = (size_t)n;

    /* ensure enough space (allocates and constructs new entries correctly)
       using the provided grow_seq helper, which works for arbitrary-precision
       seq_t types as well. */
    if (sn + 1 > cache_cap)
        grow_seq(&cache, &cache_cap, sn + 1);

    /* initialise the first three terms if they are not already present */
    if (cache_len < 3) {
        for (size_t i = cache_len; i < 3; ++i)
            cache[i] = (seq_t)1;   /* a(0)=a(1)=a(2)=1 */
        cache_len = 3;
    }

    /* compute missing terms up to the requested index (inclusive) */
    for (size_t i = cache_len; i <= sn; ++i) {
        seq_t val = (seq_t)5 * cache[i - 1]
                  - (seq_t)3 * cache[i - 2]
                  + (seq_t)7 * cache[i - 3];
        cache[i] = val;
    }

    /* update length of cached values */
    cache_len = sn + 1;

    return cache[sn];
}
