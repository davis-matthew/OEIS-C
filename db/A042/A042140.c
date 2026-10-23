/*
 * A042140  Numerators of continued fraction convergents to sqrt(595).
 * https://oeis.org/A042140
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy, less
 * Formula:   a(n) = 37028 * a(n-10) - a(n-20) for n ≥ 20. The first 20 terms
 *            are given in the specification and are loaded lazily from string
 *            literals so that the code compiles for any seq_t width
 * Range:     u8 n <= 3, u16 n <= 9, u32 n <= 19, u64 n <= 39, u128 n <= 82,
 *            big any n
 * Helpers:   seq_from_str
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/seq_from_str.h"

seq_t a(idx_t n)
{
    /* static cache for memoisation */
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;

    /* string literals of the first 20 terms */
    static const char *init_str[20] = {
        "24","49","73","122","561","683","3293","3976","7269","18514",
        "895941","1810396","2706337","4516733","20773269","25290002",
        "121933277","147223279","269156556","685536391"
    };

    /* initialise cache on first call */
    if (!cache) {
        cache_len = 20;
        cache = (seq_t *)malloc(cache_len * sizeof(seq_t));
        for (idx_t i = 0; i < cache_len; ++i)
            cache[i] = seq_from_str(init_str[i]);
    }

    /* Return value if already computed */
    if (n < cache_len)
        return cache[n];

    /* Grow cache to accommodate up to n */
    if ((idx_t)(n + 1) > cache_len) {
        cache = (seq_t *)realloc(cache, (n + 1) * sizeof(seq_t));
        cache_len = n + 1;
    }

    /* Compute missing terms using recurrence */
    for (idx_t i = 20; i <= n; ++i) {
        seq_t prod = (seq_t)37028 * cache[i - 10]; /* 37028 * a(i-10) */
        cache[i] = prod - cache[i - 20];           /* prod - a(i-20) */
    }

    return cache[n];
}
