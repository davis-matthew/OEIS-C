/*
 * A042689  Denominators of continued fraction convergents to sqrt(874).
 * https://oeis.org/A042689
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 11, u32 n <= 22, u64 n <= 39, u128 n <= 79,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A042689: Denominators of continued fraction convergents to sqrt(874)
   Formula: a(n) = 7450*a(n-8) - a(n-16)   (n > 15)
   Initial values:
   a(0)=1, a(1)=1, a(2)=2, a(3)=7, a(4)=16, a(5)=55, a(6)=71, a(7)=126,
   a(8)=7379, a(9)=7505, a(10)=14884, a(11)=52157, a(12)=119198, a(13)=409751,
   a(14)=528949, a(15)=938700
*/

#include <stdlib.h>
#include <string.h>

seq_t a(idx_t n)
{
    /* Static cache for memoization */
    static seq_t *cache   = NULL;      /* array of computed terms          */
    static size_t cache_cap = 0;       /* allocated capacity (in elements) */
    static size_t cache_len = 0;       /* number of computed terms         */

    /* Initial values for indices 0 .. 15 */
    static const seq_t initval[16] = {
        (seq_t)1,  (seq_t)1,  (seq_t)2,  (seq_t)7,
        (seq_t)16, (seq_t)55, (seq_t)71, (seq_t)126,
        (seq_t)7379, (seq_t)7505, (seq_t)14884, (seq_t)52157,
        (seq_t)119198, (seq_t)409751, (seq_t)528949, (seq_t)938700
    };

    /* Allocate cache on first call or enlarge it */
    if (!cache) {
        cache_cap = (size_t)(n + 1);
        if (cache_cap < 16) cache_cap = 16;        /* at least for the seed values   */
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache) exit(1);
    }
    if (n >= cache_len) {
        if ((size_t)(n + 1) > cache_cap) {
            size_t new_cap = cache_cap;
            while ((size_t)(n + 1) > new_cap) new_cap *= 2;
            seq_t *tmp = (seq_t *)malloc(new_cap * sizeof(seq_t));
            if (!tmp) exit(1);
            memcpy(tmp, cache, cache_cap * sizeof(seq_t));
            free(cache);
            cache = tmp;
            cache_cap = new_cap;
        }
        /* Compute missing terms sequentially */
        for (idx_t i = cache_len; i <= n; ++i) {
            if (i < 16) {
                cache[i] = initval[i];
            } else {
                cache[i] = 7450 * cache[i-8] - cache[i-16];
            }
        }
        cache_len = n + 1;
    }

    return cache[(size_t)n];
}
