/*
 * A212344  Sequence of coefficients of x^(n-3) in marked mesh pattern
 *          generating function Q_{n,132}^(0,3,0,0)(x).
 * https://oeis.org/A212344
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 5 * Catalan(n-3) Recurrence: a(3)=5, a(n) =
 *            2*(2*n-7)/(n-2) * a(n-1) for n >= 4 The function memoises values
 *            so that multiple calls in any order are efficient. It works for
 *            any build where seq_t supports normal arithmetic
 * Range:     u8 n <= 8, u16 n <= 12, u32 n <= 19, u64 n <= 29, u128 n <= 29,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    if (n < 3)   /* values below offset are unused */
        return 0;

    /* static memoisation cache */
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;

    /* initialise cache on first call */
    if (cache == NULL) {
        cache_size = (n > 3 ? n : 3);          /* at least index 3 */
        cache = (seq_t *)malloc((size_t)(cache_size + 1) * sizeof(seq_t));
        if (!cache) exit(EXIT_FAILURE);
        /* zero out lower indices; only cache[3] is needed */
        for (idx_t i = 0; i <= 3; ++i) cache[i] = 0;
        cache[3] = (seq_t)5;                   /* a(3) = 5 */
    }

    /* extend cache if needed */
    if (n > cache_size) {
        idx_t old_size = cache_size;
        cache_size = n;
        seq_t *tmp = (seq_t *)realloc(cache, (size_t)(cache_size + 1) * sizeof(seq_t));
        if (!tmp) exit(EXIT_FAILURE);
        cache = tmp;
        /* compute missing terms using recurrence */
        for (idx_t i = old_size + 1; i <= n; ++i) {
            seq_t num = (seq_t)2 * ((seq_t)(2 * i - 7));
            seq_t den = (seq_t)(i - 2);
            cache[i] = num * cache[i - 1] / den;
        }
    }

    return cache[n];
}
