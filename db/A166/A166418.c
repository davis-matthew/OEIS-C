/*
 * A166418  Number of reduced words of length n in Coxeter group on 24
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^11 = I.
 * https://oeis.org/A166418
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 22*Sum_{j=1..10} a(n-j) - 253*a(n-11) with initial values
 *            a(0)=1, a(1)=24, a(2)=552, a(3)=12696, a(4)=292008,
 *            a(5)=6716184, a(6)=154472232, a(7)=3552861336, a(8)=81715810728,
 *            a(9)=1879463646744, a(10)=43227663875112, a(11)=994236269127300
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 14, u128 n <= 28,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static const seq_t init_vals[12] = {
    (seq_t)1,
    (seq_t)24,
    (seq_t)552,
    (seq_t)12696,
    (seq_t)292008,
    (seq_t)6716184,
    (seq_t)154472232,
    (seq_t)3552861336ULL,
    (seq_t)81715810728ULL,
    (seq_t)1879463646744ULL,
    (seq_t)43227663875112ULL,
    (seq_t)994236269127300ULL
};

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;   /* number of computed entries */
    static idx_t cache_cap = 0;   /* allocated capacity */

    /* Grow cache if necessary */
    if (n + 1 > cache_cap) {
        size_t new_cap = (size_t)(n + 1);
        seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!new_cache) return 0;   /* allocation failed (unlikely) */
        cache = new_cache;
        cache_cap = new_cap;
    }

    /* Compute missing values up to n */
    for (idx_t i = cache_len; i <= n; ++i) {
        if (i < 12) {
            cache[i] = init_vals[i];
        } else {
            seq_t sum = 0;
            for (idx_t j = 1; j <= 10; ++j) sum += cache[i - j];
            cache[i] = 22 * sum - 253 * cache[i - 11];
        }
    }
    cache_len = n + 1;   /* update length of computed entries */
    return cache[n];
}
