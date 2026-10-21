/*
 * A077237  Combined Diophantine Chebyshev sequences A054491 and A077234.
 * https://oeis.org/A077237
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 4*a(n-2) - a(n-4); a(0)=1, a(1)=2, a(2)=6, a(3)=9
 * Range:     u8 n <= 7, u16 n <= 16, u32 n <= 33, u64 n <= 67, u128 n <= 133,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Compute A077237. */
seq_t a(idx_t n)
{
    /* Static cache for memoization.  cache_len is the number of values
       stored (i.e. the next index to compute). */
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;

    if (n + 1 > cache_len) {
        /* Resize cache to hold values up to n. */
        idx_t new_len = n + 1;
        cache = (seq_t *)realloc(cache, (size_t)new_len * sizeof(seq_t));
        /* Initialize newly added elements.  Loop uses idx_t because
           n is idx_t; cache indices fit in idx_t on all supported
           architectures. */
        for (idx_t i = cache_len; i <= n; ++i) {
            if (i == 0)           cache[i] = (seq_t)1;
            else if (i == 1)      cache[i] = (seq_t)2;
            else if (i == 2)      cache[i] = (seq_t)6;
            else if (i == 3)      cache[i] = (seq_t)9;
            else {
                seq_t tmp = cache[i - 2] * (seq_t)4;
                cache[i] = tmp - cache[i - 4];
            }
        }
        cache_len = new_len;
    }
    return cache[n];
}
