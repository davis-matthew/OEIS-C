/*
 * A098586  a(n) = (1/2) * (5*P(n+1) + P(n) - 1), where P(k) are the Pell
 *          numbers A000129.
 * https://oeis.org/A098586
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3*a(n-1) - a(n-2) - a(n-3) with a(0) = 2, a(1) = 5, a(2)
 *            = 13
 * Range:     u8 n <= 5, u16 n <= 11, u32 n <= 24, u64 n <= 49, u128 n <= 99,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;
    idx_t i;

    /* first time: allocate and fill up to n (or at least 3 elements) */
    if (cache == NULL) {
        idx_t init_len = (n + 1) < 3 ? 3 : (n + 1);
        cache = (seq_t *)malloc(init_len * sizeof(seq_t));
        if (cache == NULL) return 0;   /* allocation failed – return 0 as sentinel */
        cache[0] = (seq_t)2;
        cache[1] = (seq_t)5;
        cache[2] = (seq_t)13;
        for (i = 3; i < init_len; ++i)
            cache[i] = 3 * cache[i-1] - cache[i-2] - cache[i-3];
        cache_len = init_len;
    }
    /* grow cache if needed */
    else if (n >= cache_len) {
        idx_t new_len = n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
        if (tmp == NULL) return 0;   /* allocation failed – return 0 as sentinel */
        cache = tmp;
        for (i = cache_len; i < new_len; ++i)
            cache[i] = 3 * cache[i-1] - cache[i-2] - cache[i-3];
        cache_len = new_len;
    }

    return cache[n];
}
