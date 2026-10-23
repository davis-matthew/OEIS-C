/*
 * A190875  E.g.f. exp(x+x^4).
 * https://oeis.org/A190875
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1) + 4*(n-3)*(n-2)*(n-1)*a(n-4) for n ≥ 4; initial
 *            values: a(0) = a(1) = a(2) = a(3) = 1
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 13, u64 n <= 22, u128 n <= 37,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    /*  static cache for memoisation  */
    static seq_t *cache    = NULL;  /* values a(0)...a(cache_len-1) */
    static size_t cache_cap = 0;    /* allocated capacity in elements   */
    static size_t cache_len = 0;    /* number of values stored          */

    /* Allocate / grow cache to accommodate index n */
    if (cache == NULL) {
        cache_cap = (n + 4 > 4) ? (n + 4) : 4;
        cache = (seq_t*)malloc(cache_cap * sizeof(seq_t));
    } else if ((size_t)n + 1 > cache_cap) {
        size_t new_cap = cache_cap;
        while ((size_t)n + 1 > new_cap) new_cap <<= 1;
        cache = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        cache_cap = new_cap;
    }

    /* Initialise base cases if not yet done */
    if (cache_len == 0) {
        size_t init = (n < 4 ? (size_t)n + 1 : 4);
        for (size_t i = 0; i < init; ++i)
            cache[i] = (seq_t)1;
        cache_len = init;
        if (n < 4) return cache[n];
    }

    /* Compute missing values up to n */
    if (cache_len <= (size_t)n) {
        for (size_t i = cache_len; i <= (size_t)n; ++i) {
            if (i < 4) {
                cache[i] = (seq_t)1;
            } else {
                seq_t factor = (seq_t)4;
                factor *= (seq_t)(i - 3);
                factor *= (seq_t)(i - 2);
                factor *= (seq_t)(i - 1);
                cache[i] = cache[i - 1] + factor * cache[i - 4];
            }
        }
        cache_len = (size_t)n + 1;
    }

    return cache[n];
}
