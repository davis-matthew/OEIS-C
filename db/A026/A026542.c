/*
 * A026542  Expansion of 1/((1-2*x)*(1-6*x)*(1-7*x)*(1-11*x)).
 * https://oeis.org/A026542
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (11^(n+3) - 9*7^(n+3) + 9*6^(n+3) - 2^(n+3)) / 180
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 8, u64 n <= 17, u128 n <= 36,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    /* Cache array for computed terms */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;

    /* Initialize cache with first four terms if needed */
    if (cache == NULL) {
        cache_len = 4;
        cache = (seq_t*)malloc(cache_len * sizeof(seq_t));
        if (cache == NULL) abort();     /* allocation failed */
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)26;
        cache[2] = (seq_t)443;
        cache[3] = (seq_t)6292;
    }

    /* Expand cache if the requested index exceeds current length */
    if ((idx_t)(cache_len - 1) < n) {
        size_t new_len = (size_t)n + 1;
        seq_t *tmp = (seq_t*)realloc(cache, new_len * sizeof(seq_t));
        if (tmp == NULL) abort();     /* allocation failed */
        cache = tmp;
        /* Compute missing terms using the recurrence:
           a(k) = 26*a(k-1) - 233*a(k-2) + 832*a(k-3) - 924*a(k-4)   for k >= 4
        */
        for (size_t i = cache_len; i < new_len; ++i) {
            seq_t next =
                26 * cache[i - 1]
                - 233 * cache[i - 2]
                + 832 * cache[i - 3]
                - 924 * cache[i - 4];
            cache[i] = next;
        }
        cache_len = new_len;
    }

    return cache[(size_t)n];
}
