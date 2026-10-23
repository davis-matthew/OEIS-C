/*
 * A144707  Diagonal sums of the triangle A132047.
 * https://oeis.org/A144707
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = a(n-1) + 2*a(n-2) - a(n-3) - a(n-4) Initial values:
 *            a(0)=1, a(1)=1, a(2)=2, a(3)=7, a(4)=11
 * Range:     u8 n <= 9, u16 n <= 21, u32 n <= 44, u64 n <= 90, u128 n <= 182,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

seq_t a(idx_t n) {
    /* static cache for already computed terms */
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;

    /* ensure the cache holds values up to n */
    if (cache_len == 0) {
        /* start with the first 5 terms */
        cache_len = 5;
        cache = (seq_t*)malloc(cache_len * sizeof(seq_t));
        if (!cache) return 0;   /* unlikely to happen */
        cache[0] = 1;
        cache[1] = 1;
        cache[2] = 2;
        cache[3] = 7;
        cache[4] = 11;
    }

    if (n >= cache_len) {
        /* grow the cache to accommodate n */
        idx_t new_len = n + 1;
        seq_t *tmp = (seq_t*)realloc(cache, new_len * sizeof(seq_t));
        if (!tmp) return 0;   /* out of memory */
        cache = tmp;

        /* compute new terms using the recurrence */
        for (idx_t i = cache_len; i < new_len; ++i) {
            /* a(i) = a(i-1) + 2*a(i-2) - a(i-3) - a(i-4) */
            seq_t temp = cache[i-1] + cache[i-2] + cache[i-2];
            temp -= cache[i-3];
            temp -= cache[i-4];
            cache[i] = temp;
        }
        cache_len = new_len;
    }
    return cache[n];
}
