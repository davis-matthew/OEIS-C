/*
 * A227439  Number of n X 3 0,1 arrays indicating 2 X 2 subblocks of some
 *          larger (n+1) X 4 binary array having two adjacent 1's and two
 *          adjacent 0's.
 * https://oeis.org/A227439
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 11*a(n-1) - 36*a(n-2) + 79*a(n-3) - 87*a(n-4) - 7*a(n-5)
 *            * + 159*a(n-6) - 206*a(n-7) + 127*a(n-8) - 31*a(n-9) * Initial
 *            values (for n = 1 … 9): * 8, 62, 457, 3346, 24568, 180575,
 *            1327102, 9752326, 71665377
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 11, u64 n <= 22, u128 n <= 44,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* First nine terms */
    static const seq_t init[9] = {
        (seq_t)8,        (seq_t)62,
        (seq_t)457,      (seq_t)3346,
        (seq_t)24568,    (seq_t)180575,
        (seq_t)1327102,  (seq_t)9752326,
        (seq_t)71665377
    };
    if (n <= 9)
        return init[n - 1];

    /* Cache to store computed values */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;   /* allocated size */
    static size_t cache_len = 0;   /* number of elements in cache */

    /* Initialise cache on first call */
    if (cache == NULL) {
        cache_cap = 64;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache) return 0;   /* out of memory – caller should handle */
        for (int i = 0; i < 9; ++i)
            cache[i] = init[i];
        cache_len = 9;
    }

    /* Grow cache until it contains a(n) */
    while (cache_len < n) {
        /* Ensure capacity */
        if (cache_len >= cache_cap) {
            size_t new_cap = cache_cap * 2;
            seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            if (!tmp) return 0; /* out of memory */
            cache = tmp;
            cache_cap = new_cap;
        }

        /* Indices of the last nine values:
         * cache[cache_len-9] … cache[cache_len-1]
         */
        seq_t sum_pos = cache[cache_len - 1] * (seq_t)11
                      + cache[cache_len - 3] * (seq_t)79
                      + cache[cache_len - 6] * (seq_t)159
                      + cache[cache_len - 8] * (seq_t)127;

        seq_t sum_neg = cache[cache_len - 2] * (seq_t)36
                      + cache[cache_len - 4] * (seq_t)87
                      + cache[cache_len - 5] * (seq_t)7
                      + cache[cache_len - 7] * (seq_t)206
                      + cache[cache_len - 9] * (seq_t)31;

        seq_t next = sum_pos - sum_neg;   /* guaranteed non‑negative */

        cache[cache_len] = next;
        cache_len++;
    }

    return cache[n - 1];
}
