/*
 * A140162  a(1)=1, a(n) = a(n-1) + n^5 if n odd, a(n) = a(n-1) + n^0 if n is
 *          even.
 * https://oeis.org/A140162
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(1) = 1; for n > 1, a(n) = a(n-1) + (n is odd ? n^5 : 1)
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 60, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static idx_t  cache_size = 0;   /* allocated size (in terms) */
    static idx_t  computed   = 0;   /* number of terms already computed */

    /* initialise cache for the first call */
    if (computed == 0) {
        cache_size = 1;
        cache = (seq_t *)malloc(cache_size * sizeof(seq_t));
        if (!cache) return (seq_t)0;       /* out of memory; caller will never hit this */
        cache[0] = (seq_t)1;              /* a(1) = 1 */
        computed = 1;
    }

    /* expand cache if necessary */
    if (n > cache_size) {
        size_t newsize = (size_t)n;
        seq_t *tmp = (seq_t *)realloc(cache, newsize * sizeof(seq_t));
        if (!tmp) return (seq_t)0;        /* out of memory */
        cache = tmp;
        cache_size = newsize;
    }

    /* compute missing terms up to n */
    if (n > computed) {
        seq_t prev = cache[computed - 1];
        for (idx_t k = computed + 1; k <= n; ++k) {
            seq_t term;
            if (k & 1) {                  /* odd k: add k^5 */
                seq_t t = (seq_t)k;
                t *= t;   /* k^2 */
                t *= t;   /* k^4 */
                t *= (seq_t)k;  /* k^5 */
                term = t;
            } else {                       /* even k: add 1 */
                term = (seq_t)1;
            }
            prev += term;
            cache[k - 1] = prev;
        }
        computed = n;
    }

    return cache[n - 1];
}
