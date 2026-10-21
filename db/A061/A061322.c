/*
 * A061322  a(n) = a(n-1) * (1 + a(n-1)/n^2) with a(0) = 2.
 * https://oeis.org/A061322
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, changed
 * Formula:   a(n) = a(n-1) * (1 + a(n-1)/n^2), a(0) = 2
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 7, u64 n <= 8, u128 n <= 9,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Small static cache to allow arbitrary call order */
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;         /* number of terms currently stored (a(0) .. a(cache_len-1)) */

    if (cache == NULL) {               /* initialise cache with a(0) */
        cache_len = 1;
        cache = (seq_t *)malloc(sizeof(seq_t));
        *cache = (seq_t)2;             /* a(0) = 2 */
    }

    if (n + 1 > cache_len) {           /* extend cache up to index n */
        idx_t i;
        idx_t new_len = n + 1;
        cache = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
        for (i = cache_len; i < new_len; ++i) {
            seq_t prev = cache[i - 1];          /* a(i-1) */
            seq_t k    = (seq_t)i;              /* current index */
            seq_t ksq  = k * k;                 /* n^2 */
            seq_t sum  = prev + ksq;            /* a + n^2 */
            seq_t num  = prev * sum;           /* a*(a + n^2) */
            cache[i] = num / ksq;              /* exact division */
        }
        cache_len = new_len;
    }

    return cache[n];
}
