/*
 * A190360  Number of one-sided n-step prudent walks, avoiding 4 or more
 *          consecutive east steps.
 * https://oeis.org/A190360
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, walk, easy
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 12, u32 n <= 25, u64 n <= 50, u128 n <= 101,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0)=1, a(1)=3;
 * for 2 ≤ n < 4:  a(n)=2*a(n-1)+a(n-2);
 * a(4)=2*a(3)+a(2)−1;
 * for n ≥ 5:      a(n)=2*a(n-1)+a(n-2)−a(n-5)   (k=4) */
#include <stdlib.h>

seq_t a(idx_t n) {
    static seq_t *cache = NULL;
    static idx_t cache_cap = 0;
    static idx_t cache_len  = 0;
    const idx_t k = 4U;              /* k = 4 for this sequence */

    /* ensure capacity for indices up to n */
    if (n + 1 > cache_cap) {
        size_t new_cap = (cache_cap > 0) ? cache_cap * 2 : 4;
        while (new_cap < (size_t)(n + 1))
            new_cap *= 2;
        cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        cache_cap = new_cap;
    }

    /* compute any missing values up to n */
    for (idx_t i = cache_len; i <= n; ++i) {
        seq_t val;
        if (i == 0) {
            val = (seq_t)1;
        } else if (i == 1) {
            val = (seq_t)3;
        } else if (i < k) {              /* i = 2,3 */
            val = ((seq_t)2 * cache[i - 1]) + cache[i - 2];
        } else if (i == k) {             /* i = 4 */
            val = ((seq_t)2 * cache[i - 1]) + cache[i - 2] - (seq_t)1;
        } else {                         /* i ≥ 5 */
            val = ((seq_t)2 * cache[i - 1]) + cache[i - 2] - cache[i - k - 1];
        }
        cache[i] = val;
    }
    cache_len = n + 1;
    return cache[n];
}
