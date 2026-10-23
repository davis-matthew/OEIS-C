/*
 * A192917  Coefficient of x in the reduction by (x^2 -> x+1) of the
 *          polynomial C(n)*x^n, where C=A022095.
 * https://oeis.org/A192917
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*a(n-1) + 2*a(n-2) - a(n-3); a(0)=0, a(1)=5, a(2)=6
 * Range:     u8 n <= 5, u16 n <= 11, u32 n <= 22, u64 n <= 45, u128 n <= 92,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;

    if (n >= cache_size) {
        idx_t new_size = n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (!tmp) abort();
        cache = tmp;
        for (idx_t i = cache_size; i <= n; ++i) {
            if (i == 0) {
                cache[i] = (seq_t)0;
            } else if (i == 1) {
                cache[i] = (seq_t)5;
            } else if (i == 2) {
                cache[i] = (seq_t)6;
            } else {
                cache[i] = cache[i - 1] * (seq_t)2
                         + cache[i - 2] * (seq_t)2
                         - cache[i - 3];
            }
        }
        cache_size = new_size;
    }
    return cache[n];
}
