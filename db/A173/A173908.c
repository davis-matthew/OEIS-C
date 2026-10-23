/*
 * A173908  Expansion of 1/(1 + x - x^3 - x^4 - x^8 - x^12 - x^13 - x^17 -
 *          x^21 - x^22 - x^26 - x^30 - x^31 + x^33 + x^34).
 * https://oeis.org/A173908
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = -a(n-1) + a(n-3) + a(n-4) + a(n-8) + a(n-12) + a(n-13) +
 *            a(n-17) + a(n-21) + a(n-22) + a(n-26) + a(n-30) + a(n-31) -
 *            a(n-33) - a(n-34) Initial values (0 ≤ n ≤ 34): 1, -1, 1, 0, 0,
 *            0, 1, -1, 2, -2, 3, -2, 3, -2, 4, -3, 6, -5, 9, -7, 12, -9, 16,
 *            -12, 22, -17, 31, -24, 43, -33, 59, -45, 81, -...
 * Range:     i8 n <= 35, i16 n <= 69, i32 n <= 137, i64 n <= 275,
 *            i128 n <= 547, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache of previously computed terms */
    static idx_t cache_size = 0;
    static seq_t *cache = NULL;

    /* initial table of 35 terms (0..34) */
    static const seq_t base_vals[35] = {
        1,  -1,  1,   0,   0,   0,   1,   -1,    2,   -2,
        3,  -2,  3,  -2,   4,  -3,   6,  -5,    9,   -7,
       12,  -9, 16, -12,  22, -17,  31, -24,  43,  -33,
       59,  -45, 81, -63, 113
    };

    /* initialise the cache on first call */
    if (!cache) {
        cache_size = 35;
#ifdef __cplusplus
        cache = new seq_t[cache_size];
#else
        cache = (seq_t *)malloc(cache_size * sizeof(seq_t));
#endif
        for (idx_t i = 0; i < cache_size; ++i)
            cache[i] = base_vals[i];
    }

    /* return if already computed */
    if (n < cache_size)
        return cache[n];

    /* grow the cache if necessary */
    if (n >= cache_size) {
        idx_t new_size = n + 1;
#ifdef __cplusplus
        seq_t *tmp = new seq_t[new_size];
        for (idx_t i = 0; i < cache_size; ++i)
            tmp[i] = cache[i];
        delete[] cache;
#else
        seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (!tmp) /* allocation failed – terminate */
            exit(1);
#endif
        cache = tmp;
        cache_size = new_size;
    }

    /* compute missing terms iteratively */
    for (idx_t i = 35; i <= n; ++i) {
        seq_t val = -cache[i-1];
        val += cache[i-3];
        val += cache[i-4];
        val += cache[i-8];
        val += cache[i-12];
        val += cache[i-13];
        val += cache[i-17];
        val += cache[i-21];
        val += cache[i-22];
        val += cache[i-26];
        val += cache[i-30];
        val += cache[i-31];
        val -= cache[i-33];
        val -= cache[i-34];
        cache[i] = val;
    }

    return cache[n];
}
