/*
 * A159673  Expansion of 56*x^2/(1 - 783*x + 783*x^2 - x^3).
 * https://oeis.org/A159673
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) satisfies a(1)=0, a(2)=56, a(3)=43848
 *            a(n)=783*a(n-1)-783*a(n-2)+a(n-3) for n>=4
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 4, u64 n <= 8, u128 n <= 14,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    /* static cache for memoization */
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;  /* number of elements computed */
    static idx_t cache_cap = 0;  /* allocated capacity */

    if (n == 0) return 0; /* safety, offset is 1 */

    /* Ensure capacity */
    if (cache == NULL) {
        cache_cap = 4;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache) return 0; /* allocation failure, return 0 as fallback */
    }
    while (n > cache_cap) {
        cache_cap <<= 1; /* double capacity */
        cache = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
        if (!cache) return 0;
    }

    /* Compute missing terms up to n */
    for (idx_t i = cache_len; i < n; ++i) {
        if (i == 0) {
            cache[0] = (seq_t)0;                 /* a(1) */
        } else if (i == 1) {
            cache[1] = (seq_t)56;                /* a(2) */
        } else if (i == 2) {
            cache[2] = (seq_t)43848;             /* a(3) */
        } else {
            seq_t t1 = (seq_t)783 * cache[i - 1];
            seq_t t2 = (seq_t)783 * cache[i - 2];
            cache[i] = t1 - t2 + cache[i - 3];
        }
    }
    cache_len = n;

    return cache[n - 1]; /* a(n) corresponds to cache index n-1 */
}
