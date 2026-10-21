/*
 * A061403  Denominators of coefficients of even powers of x of the expansion
 *          of Bessel function J_4(x).
 * https://oeis.org/A061403
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2^(2n+4) * n! * (n+4)! Recurrence: a(0) = 384 a(n) =
 *            a(n-1) * 4 * n * (n+4) for n >= 1
 * Range:     u8 none, u16 n <= 1, u32 n <= 4, u64 n <= 7, u128 n <= 14,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache for previously computed values */
    static seq_t *cache = NULL;
    static idx_t cache_max = 0;  /* highest n already computed */

    /* initialise cache for n = 0 */
    if (cache == NULL) {
        cache = (seq_t*)malloc(sizeof(seq_t));
        if (cache == NULL) return 0;  /* out of memory – unlikely in tests */
        cache[0] = (seq_t)384;   /* a(0) = 384  */
        cache_max = 0;
    }

    /* extend cache up to requested n if necessary */
    if (n > cache_max) {
        seq_t *tmp = (seq_t*)realloc(cache, (n+1)*sizeof(seq_t));
        if (tmp == NULL) return 0;  /* allocation failed */
        cache = tmp;

        for (idx_t i = cache_max + 1; i <= n; ++i) {
            seq_t mult = (seq_t)4 * (seq_t)i * (seq_t)(i + 4);
            cache[i] = cache[i-1] * mult;
        }
        cache_max = n;
    }

    return cache[n];
}
