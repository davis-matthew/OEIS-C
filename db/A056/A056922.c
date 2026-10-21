/*
 * A056922  Denominators of continued fraction for alternating factorial.
 * https://oeis.org/A056922
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  frac, easy, nonn
 * Formula:   a(0) = 1, a(1) = 1 a(2*n) = n*a(2*n-1) + a(2*n-2) a(2*n+1) =
 *            a(2*n) + a(2*n-1)
 * Range:     u8 n <= 8, u16 n <= 13, u32 n <= 23, u64 n <= 38, u128 n <= 65,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache for memoisation */
    static seq_t *cache = NULL;
    static size_t  cache_size  = 0;  /* allocated entries          */
    static idx_t   computed_upto = 0; /* highest index that has been computed */

    /* expand the cache if necessary */
    if ((size_t)n + 1U > cache_size) {
        size_t new_size = (size_t)n + 1U;
        seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (!tmp) return (seq_t)0;      /* allocation failure */
        cache        = tmp;
        cache_size   = new_size;
    }

    /* base case a(0) = 1 */
    cache[0] = 1;
    if (n == 0) return cache[0];

    /* base case a(1) = 1 and ensure we have computed at least up to 1 */
    cache[1] = 1;
    if (computed_upto < 1) computed_upto = 1;

    /* already computed? */
    if (n <= (idx_t)computed_upto) return cache[n];

    /* compute all new terms up to n */
    for (idx_t k = computed_upto + 1; k <= n; ++k) {
        if ((k & 1U) == 0U) {                           /* even k = 2*n     */
            seq_t half = (seq_t)(k >> 1U);               /* n = k/2          */
            cache[k] = half * cache[k - 1] + cache[k - 2];
        } else {                                          /* odd k = 2*n+1    */
            cache[k] = cache[k - 1] + cache[k - 2];
        }
    }
    computed_upto = n;
    return cache[n];
}
