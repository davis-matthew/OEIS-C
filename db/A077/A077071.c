/*
 * A077071  Row sums of A077070.
 * https://oeis.org/A077071
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 0, a(1) = 2 a(2k) = a(k) + a(k-1) + 4*k*k + 2*k a(2k+1) =
 *            2*a(k) + 4*k*k + 6*k + 2
 * Range:     u8 n <= 11, u16 n <= 47, u32 n <= 47, u64 n <= 47, u128 n <= 47,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Cache to avoid recomputation; entries are computed lazily */
    static seq_t *cache = (seq_t *)NULL;
    static idx_t size   = 0;          /* number of cached entries */

    if (n >= size) {
        /* enlarge cache to hold a[n] */
        idx_t newsize = n + 1;
        cache = (seq_t *)realloc(cache, newsize * sizeof(seq_t));
        if (!cache) {                /* allocation failed – abort */
            return (seq_t)0;
        }

        /* compute missing values */
        for (idx_t i = size; i < newsize; ++i) {
            if (i == 0) {
                cache[i] = (seq_t)0;
            } else if (i == 1) {
                cache[i] = (seq_t)2;
            } else if ((i & 1) == 0) {          /* i = 2k  */
                idx_t k = i >> 1;
                seq_t kk = (seq_t)k * (seq_t)k;
                cache[i] = cache[k] + cache[k-1] + ((seq_t)4 * kk) + ((seq_t)2 * (seq_t)k);
            } else {                            /* i = 2k+1 */
                idx_t k = (i - 1) >> 1;
                seq_t kk = (seq_t)k * (seq_t)k;
                cache[i] = ((seq_t)2 * cache[k]) + ((seq_t)4 * kk)
                           + ((seq_t)6 * (seq_t)k) + (seq_t)2;
            }
        }
        size = newsize;
    }

    return cache[n];
}
