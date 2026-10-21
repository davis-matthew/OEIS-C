/*
 * A063366  Dimension of the space of weight n cuspidal newforms for Gamma_1(
 *          93 ).
 * https://oeis.org/A063366
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  sign
 * Formula:   not stated
 * Range:     i8 n <= 2, i16 n <= 41, i32 n <= 41, i64 n <= 41, i128 n <= 41,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence (derived from the rational generating function):
 *   a(n) = a(n-1) - a(n-3) + 2*a(n-4) - a(n-5) + a(n-7) - a(n-8)
 *   for n ≥ 13.  The offset of the sequence is 2 (a(2) is the first term).
 * Initial terms (used as base of the recurrence):
 *   a(2) = -1, a(3) = 209, a(4) = 450, a(5) = 690, a(6) = 930,
 *   a(7) = 1168, a(8) = 1408, a(9) = 1648, a(10) = 1886,
 *   a(11) = 2126, a(12) = 2366, a(13) = 2608.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n < 2)                     /* defined only for n ≥ 2 */
        return 0;

    idx_t idx = n - 2; /* index in the internal array (0 ↔ a(2)) */

    /* static cache for memoisation */
    static seq_t *cache   = NULL;
    static idx_t cache_len = 0;
    static idx_t cache_cap = 0;

    /* first 12 terms of the sequence (a(2) … a(13)) */
    static const seq_t init[] = {
        -1,  /* a(2) */
        209, /* a(3) */
        450, /* a(4) */
        690, /* a(5) */
        930, /* a(6) */
        1168, /* a(7) */
        1408, /* a(8) */
        1648, /* a(9) */
        1886, /* a(10) */
        2126, /* a(11) */
        2366, /* a(12) */
        2608  /* a(13) */
    };
    const idx_t init_len = sizeof(init) / sizeof(init[0]);

    /* initialise cache on first call */
    if (cache == NULL) {
        cache_cap = init_len > 16 ? init_len : 16;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache) return 0; /* allocation failure */
        for (idx_t i = 0; i < init_len; ++i)
            cache[i] = init[i];
        cache_len = init_len;
    }

    /* if the requested index is already computed, return it */
    if (idx < cache_len)
        return cache[idx];

    /* grow the cache if needed */
    if (idx + 1 > cache_cap) {
        idx_t new_cap = cache_cap ? cache_cap : 1;
        while (new_cap < idx + 1)
            new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return 0; /* allocation failure */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* compute missing terms using the recurrence */
    for (idx_t k = cache_len; k <= idx; ++k) {
        /* recurrence: a(k) = a(k-1) - a(k-3) + 2*a(k-4) - a(k-5)
         *                 + a(k-7) - a(k-8)  (for k ≥ 11, i.e. n ≥ 13)   */
        seq_t val = cache[k - 1];
        val -= cache[k - 3];
        val += (cache[k - 4] << 1);
        val -= cache[k - 5];
        val += cache[k - 7];
        val -= cache[k - 8];
        cache[k] = val;
    }

    cache_len = idx + 1;
    return cache[idx];
}
