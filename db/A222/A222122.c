/*
 * A222122  Number of binary arrays indicating the locations of trailing edge
 *          maxima of a random length-n 0..3 array extended with zeros and
 *          convolved with 1,2,1.
 * https://oeis.org/A222122
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1) + a(n-3) + a(n-5) (for n >= 6) Initial values:
 *            a(1)=2, a(2)=3, a(3)=4, a(4)=7, a(5)=11, a(6)=17
 * Range:     u8 n <= 11, u16 n <= 24, u32 n <= 48, u64 n <= 98, u128 n <= 196,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t max_cached = 0;   /* maximum n stored in cache (0 if not yet allocated) */

    /* allocate minimal cache and store the six initial values on the first call */
    if (!cache) {
        cache = (seq_t *)calloc(7, sizeof(seq_t));   /* indices 0..6, index 0 unused */
        max_cached = 6;
        cache[1] = (seq_t)2;
        cache[2] = (seq_t)3;
        cache[3] = (seq_t)4;
        cache[4] = (seq_t)7;
        cache[5] = (seq_t)11;
        cache[6] = (seq_t)17;
    }

    /* if the requested n is beyond what we have computed, extend the cache */
    if ((idx_t)max_cached < n) {
        size_t new_cap = (size_t)n + 1;          /* one past the largest index used */
        cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        for (idx_t i = (idx_t)max_cached + 1; i <= n; ++i) {
            if (i <= 6)  /* already initialized */
                continue;
            cache[i] = cache[i-1] + cache[i-3] + cache[i-5];
        }
        max_cached = (size_t)n;
    }

    return cache[n];
}
