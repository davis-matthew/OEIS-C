/*
 * A077138  a(0) = 0. If n is odd, a(n) = a(n-1) + n, otherwise a(n) = a(n-1)
 *          * n.
 * https://oeis.org/A077138
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0) = 0, a(n) = a(n-1)+n if n is odd, a(n) = a(n-1)*n if n is
 *            even
 * Range:     u8 n <= 7, u16 n <= 11, u32 n <= 19, u64 n <= 33, u128 n <= 57,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Static cache: a(0) = 0 stored at cache[0] */
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;      /* number of cached terms, always >=1 after init */

    /* Initialize cache with a(0) on first call */
    if (cache_len == 0) {
        cache = (seq_t *)malloc(sizeof(seq_t));
        if (!cache) return (seq_t)0;   /* allocation failure: return 0 */
        cache[0] = (seq_t)0;
        cache_len = 1;
    }

    /* Grow cache up to n if needed */
    if (n + 1 > cache_len) {
        idx_t old_len = cache_len;
        idx_t new_len = n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, (size_t)new_len * sizeof(seq_t));
        if (!tmp) return cache[old_len - 1];   /* on failure return last known term */
        cache = tmp;
        cache_len = new_len;

        /* Compute and store missing terms */
        for (idx_t i = old_len; i <= n; ++i) {
            if ((i & 1) != 0) {          /* odd */
                cache[i] = cache[i-1] + (seq_t)i;
            } else {                      /* even */
                cache[i] = cache[i-1] * (seq_t)i;
            }
        }
    }

    return cache[n];
}
