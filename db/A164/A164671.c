/*
 * A164671  Number of reduced words of length n in Coxeter group on 35
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^7 = I.
 * https://oeis.org/A164671
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 33*(a(n-1)+a(n-2)+a(n-3)+a(n-4)+a(n-5)+a(n-6)) -
 *            561*a(n-7) for n≥8, with a(0) = 1, a(1) = 35, a(2) = 1190, a(3)
 *            = 40460, a(4) = 1375640, a(5) = 46771760, a(6) = 1590239840,
 *            a(7) = 33*(a6+…+a1) – 560
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 12, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;

    /* initialise cache with first 8 terms (a0 … a7) */
    if (!cache) {
        cache_len = 8;                            /* indices 0 … 7 */
        cache = (seq_t *)malloc(cache_len * sizeof(seq_t));
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)35;
        cache[2] = (seq_t)1190;
        cache[3] = (seq_t)40460;
        cache[4] = (seq_t)1375640;
        cache[5] = (seq_t)46771760;
        cache[6] = (seq_t)1590239840;
        /* compute a(7) from the generating function: 33*sum – 560 */
        seq_t sum = cache[6] + cache[5] + cache[4] + cache[3] + cache[2] + cache[1];
        cache[7] = 33 * sum - (seq_t)560;
    }

    /* return early if already computed */
    if (n < cache_len) {
        return cache[n];
    }

    /* grow cache to accommodate up to index n */
    cache_len = n + 1;
    cache = (seq_t *)realloc(cache, cache_len * sizeof(seq_t));

    /* compute remaining terms using the linear recurrence */
    for (idx_t i = 8; i <= n; ++i) {
        seq_t s = cache[i-1] + cache[i-2] + cache[i-3]
                + cache[i-4] + cache[i-5] + cache[i-6];
        cache[i] = 33 * s - (seq_t)561 * cache[i-7];
    }

    return cache[n];
}
