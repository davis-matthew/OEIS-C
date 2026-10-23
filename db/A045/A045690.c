/*
 * A045690  Number of binary words of length n (beginning with 0) whose
 *          autocorrelation function is the indicator of a singleton.
 * https://oeis.org/A045690
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   a(1)=1, a(2n)=2*a(2n-1)-a(n), a(2n+1)=2*a(2n)
 * Range:     u8 n <= 10, u16 n <= 18, u32 n <= 34, u64 n <= 66, u128 n <= 130,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)0;          /* undefined offset, harmless */

    static seq_t *cache = NULL;           /* memoisation storage */
    static idx_t cache_size = 0;          /* allocated capacity    */
    static idx_t computed = 0;            /* highest n already known */

    /* grow cache to hold up to n */
    if (n > cache_size) {
        seq_t *new_cache = (seq_t *)malloc((size_t)n * sizeof(seq_t));
        if (!new_cache) return (seq_t)0;  /* allocation failure */
        for (idx_t i = 0; i < cache_size; ++i)
            new_cache[i] = cache[i];
        if (cache) free(cache);
        cache = new_cache;
        cache_size = n;
    }

    /* compute any missing terms up to n */
    for (idx_t i = computed + 1; i <= n; ++i) {
        if (i == 1) {
            cache[0] = (seq_t)1;
        } else if ((i & 1) == 0) {              /* i even: 2n */
            idx_t k = i >> 1;                   /* k = i/2 */
            cache[i-1] = (seq_t)2 * cache[i-2] - cache[k-1];
        } else {                                 /* i odd: 2n+1 */
            cache[i-1] = (seq_t)2 * cache[i-2];
        }
    }
    computed = n;
    return cache[n-1];
}
