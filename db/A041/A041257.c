/*
 * A041257  Denominators of continued fraction convergents to sqrt(140).
 * https://oeis.org/A041257
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   a(n) = 142 * a(n-4) - a(n-8) Initial values: a(0)=1, a(1)=1,
 *            a(2)=5, a(3)=6, a(4)=137, a(5)=143, a(6)=709, a(7)=852
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 19, u64 n <= 35, u128 n <= 71,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;      /* static cache of computed terms   */
    static idx_t cache_len = 0;      /* number of terms stored (size of cache) */

    /* first eight terms, used for initialisation */
    static const seq_t init[8] = { (seq_t)1, (seq_t)1, (seq_t)5, (seq_t)6,
                                   (seq_t)137, (seq_t)143, (seq_t)709, (seq_t)852 };

    /* initialise cache on first call */
    if (cache == NULL) {
        cache_len = 8;
        cache = (seq_t*)malloc(cache_len * sizeof(seq_t));
        for (idx_t i = 0; i < cache_len; ++i) {
            cache[i] = init[i];
        }
    }

    /* enlarge cache if necessary */
    if (n >= cache_len) {
        idx_t new_len = n + 1;
        cache = (seq_t*)realloc(cache, new_len * sizeof(seq_t));

        /* compute new terms using the recurrence */
        for (idx_t i = cache_len; i <= n; ++i) {
            cache[i] = (seq_t)142 * cache[i-4] - cache[i-8];
        }
        cache_len = new_len;
    }

    return cache[n];
}
