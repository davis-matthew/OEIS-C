/*
 * A083563  Number of binary rooted trees (every node has out-degree 0 or 2)
 *          with n labeled leaves (2n-1 nodes in all) and at most 2 distinct
 *          labels. Also the number of expressions in at most two variables
 *          constructible with n-1 instances of a single commutative and
 *          nonassociative binary operator.
 * https://oeis.org/A083563
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, eigen, nonn
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 10, u32 n <= 18, u64 n <= 33, u128 n <= 64,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A083563: number of binary rooted trees with n labeled leaves
   and at most two distinct labels.
   Recurrence:
     a(0) = 0
     a(1) = 2
     For n = 2m (even):
       a(n) = Σ_{k=1}^{m-1} a(k) * a(n - k) + a(m) * (a(m) + 1) / 2
     For n = 2m - 1 (odd):
       a(n) = Σ_{k=1}^{m-1} a(k) * a(n - k)
*/
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Static cache for memoised values */
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;   /* number of entries stored (== cache_size) */

    /* Return cached value if already computed */
    if (n < cache_size)
        return cache[n];

    /* Grow cache to accommodate up to n */
    idx_t new_size = n + 1;
    cache = (seq_t *)realloc(cache, new_size * sizeof(seq_t));

    /* Compute new values from cache_size up to n */
    for (idx_t i = cache_size; i <= n; ++i) {
        if (i == 0) {
            cache[0] = (seq_t)0;
            continue;
        }
        if (i == 1) {
            cache[1] = (seq_t)2;
            continue;
        }

        if (i & 1) { /* odd: i = 2m - 1 */
            idx_t m      = (i + 1) >> 1;       /* m = (i+1)/2 */
            seq_t sum    = (seq_t)0;
            for (idx_t k = 1; k < m; ++k)
                sum += cache[k] * cache[i - k];
            cache[i] = sum;
        } else {   /* even: i = 2m */
            idx_t m      = i >> 1;              /* m = i/2 */
            seq_t sum    = (seq_t)0;
            for (idx_t k = 1; k < m; ++k)
                sum += cache[k] * cache[i - k];
            seq_t extra = cache[m];
            sum += extra * (extra + (seq_t)1) / (seq_t)2; /* a(m)*(a(m)+1)/2 */
            cache[i] = sum;
        }
    }

    cache_size = new_size;
    return cache[n];
}
