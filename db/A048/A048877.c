/*
 * A048877  a(n) = 4*a(n-1) + a(n-2); a(0)=1, a(1)=8.
 * https://oeis.org/A048877
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 4*a(n-1) + a(n-2); a(0) = 1, a(1) = 8
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 14, u64 n <= 30, u128 n <= 61,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>

seq_t a(idx_t n)
{
    /* Static cache to avoid recomputation */
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;

    /* Initialise cache with the first two terms */
    if (cache == NULL) {
        cache_len = 2;
        cache = (seq_t *)malloc(cache_len * sizeof(seq_t));
        if (!cache) abort();          /* memory allocation failed */
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)8;
    }

    /* Grow cache if needed */
    if (n >= cache_len) {
        idx_t new_len = cache_len;
        while (new_len <= n) {
            new_len = new_len * 2;
            if (new_len == 0) new_len = n + 1;  /* in case cache_len was 0 */
        }
        seq_t *tmp = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
        if (!tmp) abort();
        cache = tmp;
        cache_len = new_len;
    }

    /* Compute terms up to n */
    for (idx_t i = (n >= 2 ? 2 : (idx_t)cache_len); i <= n; ++i) {
        cache[i] = cache[i-1] * (seq_t)4 + cache[i-2];
    }

    return cache[n];
}
