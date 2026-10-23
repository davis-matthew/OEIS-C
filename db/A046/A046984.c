/*
 * A046984  Number of ways to tile a 4 X 3n rectangle with right trominoes.
 * https://oeis.org/A046984
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   a(0)=1, a(1)=4, a(2)=18, a(n)=10*a(n-1)-22*a(n-2)-4*a(n-3) for
 *            n>=3
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 12, u64 n <= 22, u128 n <= 22,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cache_sz = 0;

    if ((idx_t)cache_sz <= n) {
        size_t new_sz = (size_t)(n + 1);
        seq_t *tmp = (seq_t *)realloc(cache, new_sz * sizeof(seq_t));
        if (!tmp) {
            /* Allocation failed – return zero to avoid crash */
            return (seq_t)0;
        }
        cache = tmp;
        for (idx_t i = (idx_t)cache_sz; i <= n; ++i) {
            if (i == 0) {
                cache[i] = (seq_t)1;
            } else if (i == 1) {
                cache[i] = (seq_t)4;
            } else if (i == 2) {
                cache[i] = (seq_t)18;
            } else {
                seq_t t = (seq_t)10 * cache[i - 1];
                t -= (seq_t)22 * cache[i - 2];
                t -= (seq_t)4 * cache[i - 3];
                cache[i] = t;
            }
        }
        cache_sz = new_sz;
    }
    return cache[n];
}
