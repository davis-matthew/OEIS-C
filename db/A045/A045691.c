/*
 * A045691  Number of binary words of length n with autocorrelation function
 *          2^(n-1)+1.
 * https://oeis.org/A045691
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 0, a(1) = 1; a(2*n-1) = 2*a(2*n-2) + a(n-1) for n ≥ 2,
 *            a(2*n) = 2*a(2*n-1) - a(n) for n ≥ 2
 * Range:     u8 n <= 9, u16 n <= 17, u32 n <= 33, u64 n <= 34, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache of previously computed values */
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;

    /* ensure cache exists with at least indices 0 and 1 */
    if (cache == NULL) {
        cache_size = 2;                       /* indices 0 .. 1 */
        cache = (seq_t *)malloc(sizeof(seq_t) * cache_size);
        if (!cache) return 0;                 /* unlikely but safe */
        cache[0] = (seq_t)0;
        cache[1] = (seq_t)1;
    }

    if (n + 1 > cache_size) {
        /* grow cache to required size, computing new entries on the fly */
        idx_t new_size = n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, sizeof(seq_t) * new_size);
        if (!tmp) return 0;                 /* unlikely but safe */
        cache = tmp;

        for (idx_t i = cache_size; i <= n; ++i) {
            seq_t val;
            if (i == 0) {
                val = (seq_t)0;
            } else if (i == 1) {
                val = (seq_t)1;
            } else if ((i & 1) == 0) {
                /* i is even: i = 2 * k  with k ≥ 2 */
                idx_t k = i / 2;
                val = (cache[i - 1] << 1) - cache[k];
            } else {
                /* i is odd: i = 2 * k - 1  with k ≥ 2 */
                idx_t k = (i + 1) / 2;     /* k = (i+1)/2 ≥ 2 */
                val = (cache[i - 1] << 1) + cache[k - 1];
            }
            cache[i] = val;
        }

        cache_size = new_size;
    }

    return cache[n];
}
