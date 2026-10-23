/*
 * A190029  Number of (n+2) X 7 binary arrays with every 3 X 3 subblock
 *          commuting with each horizontal and vertical neighbor 3 X 3
 *          subblock.
 * https://oeis.org/A190029
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*a(n-1) - a(n-2) + a(n-3) - 2*a(n-4) + a(n-5) + a(n-6) -
 *            a(n-7) for n > 13, with initial values: a(1)=616, a(2)=235,
 *            a(3)=362, a(4)=578, a(5)=834, a(6)=970, a(7)=1162, a(8)=1418,
 *            a(9)=1802, a(10)=2446, a(11)=3314, a(12)=4446, a(13)=6199
 * Range:     u8 none, u16 n <= 20, u32 n <= 56, u64 n <= 129, u128 n <= 200,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    /* static cache for memoization */
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;
    /* initial 13 terms (index 0 unused) */
    static const seq_t init[14] = {0,
        616, 235, 362, 578, 834, 970, 1162, 1418,
        1802, 2446, 3314, 4446, 6199};

    /* allocate and load cache if first call */
    if (cache == NULL) {
        cache_size = 13;
        cache = (seq_t*)malloc((cache_size + 1) * sizeof(seq_t));
        if (cache == NULL) abort();
        for (idx_t i = 1; i <= cache_size; ++i)
            cache[i] = init[i];
    }

    /* extend cache up to requested n */
    if (n > cache_size) {
        idx_t new_size = n;
        seq_t *new_cache = (seq_t*)realloc(cache, (new_size + 1) * sizeof(seq_t));
        if (new_cache == NULL) abort();
        cache = new_cache;
        for (idx_t i = cache_size + 1; i <= new_size; ++i) {
            if (i <= 13) {
                cache[i] = init[i];
            } else {
                seq_t a1 = cache[i - 1];
                seq_t a2 = cache[i - 2];
                seq_t a3 = cache[i - 3];
                seq_t a4 = cache[i - 4];
                seq_t a5 = cache[i - 5];
                seq_t a6 = cache[i - 6];
                seq_t a7 = cache[i - 7];
                /* compute 2*a1 + a3 + a5 + a6 - a2 - 2*a4 - a7 */
                seq_t sum = a1 + a1 + a3 + a5 + a6;
                sum -= a2;
                sum -= a4;
                sum -= a4;
                sum -= a7;
                cache[i] = sum;
            }
        }
        cache_size = new_size;
    }

    return cache[n];
}
