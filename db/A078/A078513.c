/*
 * A078513  a(0)=0, a(1)=1, a(n)=a(n-1)+a(n-2)+a(n-3) if a(n-1) is even,
 *          a(n)=a(n-1)+a(n-2) if a(n-1) is odd.
 * https://oeis.org/A078513
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(0) = 0, a(1) = 1, a(n) = a(n‑1)+a(n‑2)+a(n‑3) if a(n‑1) is
 *            even, a(n) = a(n‑1)+a(n‑2) if a(n‑1) is odd
 * Range:     u8 n <= 11, u16 n <= 21, u32 n <= 36, u64 n <= 36, u128 n <= 36,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    /* static cache to enable out‑of‑order calls */
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;
    static idx_t cache_cap = 0;

    /* initialize cache */
    if (cache_len == 0) {
        cache_cap = 4;
        cache = (seq_t *)malloc(sizeof(seq_t) * cache_cap);
        if (!cache) return (seq_t)0;        /* fallback, should not happen */
        cache[0] = (seq_t)0;
        cache[1] = (seq_t)1;
        cache_len = 2;
    }

    /* grow cache if needed */
    while (cache_len <= n) {
        if (cache_len == cache_cap) {
            cache_cap = cache_cap * 2;
            cache = (seq_t *)realloc(cache, sizeof(seq_t) * cache_cap);
            if (!cache) return (seq_t)0;    /* fallback, should not happen */
        }

        idx_t i = cache_len;          /* next term to compute */

        seq_t val;
        if (i == 2) {
            /* a(2) = a(1)+a(0) because a(1) is odd */
            val = cache[i - 1] + cache[i - 2];
        } else {
            /* i >= 3 */
            if ((cache[i - 1] & (seq_t)1) == 0) {
                /* even last term */
                val = cache[i - 1] + cache[i - 2] + cache[i - 3];
            } else {
                /* odd last term */
                val = cache[i - 1] + cache[i - 2];
            }
        }

        cache[i] = val;
        cache_len = i + 1;
    }

    return cache[n];
}
