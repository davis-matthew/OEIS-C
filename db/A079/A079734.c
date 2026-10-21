/*
 * A079734  n for which there is a chain (or permutation) of the numbers from
 *          1 to n for which each adjacent pair sums to a Fibonacci number.
 * https://oeis.org/A079734
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 2*a(n-2) - a(n-6) for n > 14 Initial values: a(1)=2,
 *            a(2)=3, a(3)=4, a(4)=5, a(5)=7, a(6)=8, a(7)=9, a(8)=11,
 *            a(9)=12, a(10)=13, a(11)=20, a(12)=21, a(13)=33, a(14)=34
 * Range:     u8 n <= 22, u16 n <= 44, u32 n <= 90, u64 n <= 182,
 *            u128 n <= 368, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    if (n == 0) { /* sequence is 1‑based; n>=1 is guaranteed, but guard */
        return 0;
    }
    /* static cache of computed terms */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;
    static size_t cache_cap = 0;

    /* extend cache if needed */
    if ((size_t)n > cache_len) {
        size_t new_cap = cache_cap ? cache_cap : 16;
        while (new_cap < (size_t)n) new_cap <<= 1;
        seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) { /* allocation failure – abort */
            exit(EXIT_FAILURE);
        }
        cache = tmp;
        cache_cap = new_cap;
    }

    /* initialize initial terms up to n=14 */
    if (cache_len < 14) {
        /* explicit values for indices 1..14 */
        seq_t init[14] = {2,3,4,5,7,8,9,11,12,13,20,21,33,34};
        size_t up = 14;
        if (up > cache_len) {
            for (size_t i = cache_len; i < up; ++i)
                cache[i] = init[i];
            cache_len = up;
        }
    }

    /* compute further terms using recurrence */
    for (size_t i = cache_len + 1; i <= (size_t)n; ++i) {
        /* i is 1‑based index of term to compute */
        seq_t val = 2 * cache[i - 3] - cache[i - 7];
        cache[i - 1] = val;
    }
    cache_len = (size_t)n;

    return cache[n - 1];
}
