/*
 * A068921  Number of ways to tile a 2 X n room with 1 X 2 Tatami mats. At
 *          most 3 Tatami mats may meet at a point.
 * https://oeis.org/A068921
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1) + a(n-3), a(0)=1, a(1)=1, a(2)=2
 * Range:     u8 n <= 14, u16 n <= 29, u32 n <= 58, u64 n <= 116,
 *            u128 n <= 232, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache for memoisation */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;
    size_t target = (size_t)n;

    /* initialise cache on first call */
    if (cache_len == 0) {
        cache_len = 3;
        cache = (seq_t*)malloc(cache_len * sizeof(seq_t));
        if (!cache) return 0;          /* out‑of‑memory – not expected */
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)1;
        cache[2] = (seq_t)2;
    }

    /* extend cache if needed */
    if (target >= cache_len) {
        size_t new_len = target + 1;
        seq_t *tmp = (seq_t*)realloc(cache, new_len * sizeof(seq_t));
        if (!tmp) return 0;           /* out‑of‑memory – not expected */
        cache = tmp;
        for (size_t i = cache_len; i <= target; ++i) {
            cache[i] = cache[i - 1] + cache[i - 3];
        }
        cache_len = new_len;
    }

    return cache[target];
}
