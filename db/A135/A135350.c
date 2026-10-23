/*
 * A135350  a(n) = 2*a(n-1) - a(n-3) + 2*a(n-4).
 * https://oeis.org/A135350
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 16, u32 n <= 32, u64 n <= 64, u128 n <= 128,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n) = 2*a(n-1) - a(n-3) + 2*a(n-4),
   a(0)=0, a(1)=1, a(2)=3, a(3)=8 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cache_len = 0;
    size_t i;

    /* Ensure base terms are stored */
    if (cache_len == 0) {
        cache_len = 4;
        cache = (seq_t *)malloc(cache_len * sizeof(seq_t));
        cache[0] = (seq_t)0;
        cache[1] = (seq_t)1;
        cache[2] = (seq_t)3;
        cache[3] = (seq_t)8;
    }

    /* Grow cache if necessary and compute missing values */
    if ((size_t)n >= cache_len) {
        size_t new_len = (size_t)n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
        if (!tmp) {
            /* Memory allocation failed – return 0 as a fallback */
            return (seq_t)0;
        }
        cache = tmp;
        for (i = cache_len; i <= (size_t)n; ++i) {
            seq_t t1 = cache[i - 1];
            t1 = t1 + t1;          /* 2*a(i-1) */
            seq_t t2 = cache[i - 4];
            t2 = t2 + t2;          /* 2*a(i-4) */
            cache[i] = t1 - cache[i - 3] + t2;
        }
        cache_len = new_len;
    }

    return cache[(size_t)n];
}
