/*
 * A143456  Expansion of 1/(x^k*(1-x-3*x^(k+1))) for k=5.
 * https://oeis.org/A143456
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1) + 3*a(n-6) (n >= 6) Initial terms: a(0)=1, a(1)=4,
 *            a(2)=7, a(3)=10, a(4)=13, a(5)=16
 * Range:     u8 n <= 12, u16 n <= 26, u32 n <= 56, u64 n <= 115,
 *            u128 n <= 233, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>  /* for size_t */

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cache_len = 0;

    /* Ensure base cases are initialized once */
    if (cache_len == 0) {
        cache_len = 6;
        cache = (seq_t*)malloc(cache_len * sizeof(seq_t));
        if (!cache) abort();
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)4;
        cache[2] = (seq_t)7;
        cache[3] = (seq_t)10;
        cache[4] = (seq_t)13;
        cache[5] = (seq_t)16;
    }

    /* Extend cache to cover n if necessary */
    if ((size_t)n >= cache_len) {
        size_t new_len = (size_t)n + 1;
        seq_t *tmp = (seq_t*)realloc(cache, new_len * sizeof(seq_t));
        if (!tmp) abort();
        cache = tmp;
        for (size_t i = cache_len; i <= (size_t)n; ++i) {
            /* i > 5 because cache_len is at least 6 */
            cache[i] = cache[i-1] + (seq_t)3 * cache[i-6];
        }
        cache_len = new_len;
    }

    return cache[(size_t)n];
}
