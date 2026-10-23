/*
 * A026397  Sum{T(n-k,k)}, 0<=k<=[ n/2 ], where T is the array in A026386.
 * https://oeis.org/A026397
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-2) + 3*a(n-3) + a(n-4) for n≥4, with initial values
 *            a(0)=1, a(1)=2, a(2)=3, a(3)=6
 * Range:     u8 n <= 9, u16 n <= 19, u32 n <= 33, u64 n <= 33, u128 n <= 33,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;   /* dynamically grown cache of terms */
    static size_t  cap   = 0;     /* allocated capacity of cache      */

    size_t needed = (size_t)n + 1;          /* number of terms required */
    if (needed > cap) {
        size_t old_cap = cap;
        seq_t *new_cache = (seq_t *)malloc(needed * sizeof(seq_t));
        if (!new_cache) return (seq_t)0;    /* allocation failure (undefined) */

        /* copy previously computed values */
        if (cache) {
            memcpy(new_cache, cache, old_cap * sizeof(seq_t));
            free(cache);
        }
        cache = new_cache;
        cap   = needed;

        /* fill the new entries */
        for (size_t i = old_cap; i < cap; ++i) {
            idx_t idx = (idx_t)i;
            if (idx == 0)          cache[i] = (seq_t)1;
            else if (idx == 1)     cache[i] = (seq_t)2;
            else if (idx == 2)     cache[i] = (seq_t)3;
            else if (idx == 3)     cache[i] = (seq_t)6;
            else
                cache[i] = cache[i-2] + (seq_t)3 * cache[i-3] + cache[i-4];
        }
    }
    return cache[n];
}
