/*
 * A212890  Number of (w,x,y,z) with all terms in {0,...,n} and odd range.
 * https://oeis.org/A212890
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) =
 *            2*a(n-1)+2*a(n-2)-6*a(n-3)+6*a(n-5)-2*a(n-6)-2*a(n-7)+a(n-8),
 *            a(0)=0, a(1)=14, a(2)=28, a(3)=152, a(4)=276, a(5)=702,
 *            a(6)=1128, a(7)=2144. This recurrence uses only non-negative
 *            intermediate sums (sumPos ≥ sumNeg), so unsigned arithmetic is
 *            safe
 * Range:     u8 n <= 3, u16 n <= 18, u32 n <= 33, u64 n <= 33, u128 n <= 33,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    /* Static cache of already-computed values */
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;   /* number of elements allocated   */

    /* Initial values for n = 0..7 */
    static const seq_t init[8] = {
        0, 14, 28, 152, 276, 702, 1128, 2144
    };

    /* Grow the cache if necessary */
    if (n + 1 > cache_size) {
        seq_t *tmp = (seq_t *)realloc(cache, (n + 1) * sizeof(seq_t));
        if (!tmp) return (seq_t)0;   /* allocation failure: return 0 */
        cache = tmp;
        /* Initialize new entries to 0 */
        for (idx_t i = cache_size; i < n + 1; ++i) cache[i] = 0;
        cache_size = n + 1;
    }

    /* Compute values up to n */
    for (idx_t i = 0; i <= n; ++i) {
        if (i < 8) {
            if (cache[i] != init[i]) cache[i] = init[i];
            continue;
        }
        /* sumPos = 2*a[i-1] + 2*a[i-2] + 6*a[i-5] + a[i-8] */
        seq_t sumPos = 2 * cache[i - 1] + 2 * cache[i - 2]
                      + 6 * cache[i - 5] + cache[i - 8];
        /* sumNeg = 6*a[i-3] + 2*a[i-6] + 2*a[i-7] */
        seq_t sumNeg = 6 * cache[i - 3] + 2 * cache[i - 6]
                      + 2 * cache[i - 7];
        cache[i] = sumPos - sumNeg;  /* sumPos ≥ sumNeg by definition */
    }

    return cache[n];
}
