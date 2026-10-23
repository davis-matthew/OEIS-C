/*
 * A154232  a(2n) = (n^2-n-1) + a(2n-2), a(2n+1) = (n^2-n-1)*a(2n-1), with
 *          a(0)=0 and a(1)=1.
 * https://oeis.org/A154232
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(0) = 0, a(1) = 1 a(2n) = (n^2 - n - 1) + a(2n-2) a(2n+1) =
 *            (n^2 - n - 1) * a(2n-1) for n ≥ 1
 * Range:     i8 n <= 10, i16 n <= 14, i32 n <= 18, i64 n <= 28, i128 n <= 44,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static memoisation cache */
    static seq_t *cache = NULL;      /* array of already computed values   */
    static idx_t  cache_size = 0;    /* number of elements allocated      */

    /* initialise the first two terms on the first call */
    if (cache_size == 0) {
        cache = (seq_t *)malloc(2 * sizeof(seq_t));
        if (!cache) return 0;          /* allocation failure opaque to caller */
        cache[0] = (seq_t)0;
        cache[1] = (seq_t)1;
        cache_size = 2;
    }

    /* enlarge cache if required to hold 0 … n */
    if (n + 1 > cache_size) {
        seq_t *tmp = (seq_t *)realloc(cache, (n + 1) * sizeof(seq_t));
        if (!tmp) return 0;           /* allocation failure */
        cache = tmp;
        cache_size = n + 1;
    }

    /* compute missing terms in increasing order */
    for (idx_t i = 2; i <= n; ++i) {
        if (i < cache_size && i < 2) continue; /* skip the already set first two */
        seq_t coeff;          /* n^2 - n - 1  */
        if ((i & 1) == 0) {   /* i = 2k (even)  */
            seq_t k = (seq_t)(i / 2);
            coeff = k * k - k - (seq_t)1;
            cache[i] = coeff + cache[i - 2];
        } else {              /* i = 2k+1 (odd) */
            seq_t k = (seq_t)(i / 2);
            coeff = k * k - k - (seq_t)1;
            cache[i] = coeff * cache[i - 2];
        }
    }

    return cache[n];
}
