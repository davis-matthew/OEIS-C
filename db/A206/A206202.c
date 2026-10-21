/*
 * A206202  Number of (n+1)X5 0..3 arrays with every 2X3 or 3X2 subblock
 *          having exactly one clockwise edge increases.
 * https://oeis.org/A206202
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 10*a(n-3) + a(n-9) for n > 18 Initial values (for 1 ≤ n ≤
 *            18): 7388, 9464, 11832, 32920, 70168, 109720, 337896, 728704,
 *            1137208, 3418192, 7313872, 11389472, 34215016, 73207664,
 *            114003240, 342487240, 732805056, 1141169536
 * Range:     u8 none, u16 n <= 4, u32 n <= 19, u64 n <= 48, u128 n <= 105,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

static const seq_t seed[18] = {
    (seq_t)7388,      (seq_t)9464,      (seq_t)11832,
    (seq_t)32920,     (seq_t)70168,     (seq_t)109720,
    (seq_t)337896,    (seq_t)728704,    (seq_t)1137208,
    (seq_t)3418192,   (seq_t)7313872,   (seq_t)11389472,
    (seq_t)34215016,  (seq_t)73207664,  (seq_t)114003240,
    (seq_t)342487240, (seq_t)732805056, (seq_t)1141169536
};

seq_t a(idx_t n)
{
    if (n < 1) return 0;                     /* undefined for n < 1 */

    /* Allocate once and memoise computed values */
    static seq_t *cache = NULL;
    static size_t cache_size = 0;
    static idx_t max_cached = 0;

    if (cache == NULL) {                       /* first call */
        cache_size = 64;                       /* start with some space */
        cache = (seq_t *)malloc(cache_size * sizeof(seq_t));
        if (!cache) return 0;                  /* allocation failed */
        for (size_t i = 0; i < cache_size; ++i) cache[i] = 0;
        /* initialise the 18 known terms */
        for (idx_t i = 1; i <= 18; ++i) cache[i] = seed[i-1];
        max_cached = 18;
    }

    /* expand cache if needed */
    if (n >= cache_size) {
        size_t new_size = cache_size;
        while (n >= new_size) new_size <<= 1;  /* double until sufficient */
        seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (!tmp) return 0;                    /* allocation failed */
        for (size_t i = cache_size; i < new_size; ++i) tmp[i] = 0;
        cache = tmp;
        cache_size = new_size;
    }

    /* compute any missing values up to n */
    if (n > max_cached) {
        for (idx_t i = max_cached + 1; i <= n; ++i) {
            if (i > 18) {
                cache[i] = (seq_t)10 * cache[i-3] + cache[i-9];
            }
        }
        max_cached = n;
    }

    return cache[n];
}
