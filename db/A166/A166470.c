/*
 * A166470  a(n) = 2^F(n+1)*3^F(n), where F(n) is the n-th Fibonacci number,
 *          A000045(n).
 * https://oeis.org/A166470
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1) * a(n-2); a(0) = 2, a(1) = 6
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 6, u64 n <= 7, u128 n <= 9,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;   /* previously computed values */
    static idx_t cache_len = 0;   /* number of stored elements */

    /* extend the cache up to index n if necessary */
    if (n >= cache_len) {
        idx_t new_len = n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, sizeof(seq_t) * (size_t)new_len);
        if (!tmp && new_len > 0) {
            /* allocation failure – return a neutral value for safety */
            return (seq_t)0;
        }
        cache = tmp;
        for (idx_t i = cache_len; i <= n; ++i) {
            if (i == 0)
                cache[i] = (seq_t)2;
            else if (i == 1)
                cache[i] = (seq_t)6;
            else
                cache[i] = cache[i-1] * cache[i-2];
        }
        cache_len = new_len;
    }
    return cache[n];
}
