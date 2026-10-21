/*
 * A220186  Numbers n >= 0 such that n^2 + n*(n+1)/2 is a square.
 * https://oeis.org/A220186
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 99*a(n-1) - 99*a(n-2) + a(n-3), with a(1)=0, a(2)=8,
 *            a(3)=800
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 6, u64 n <= 11, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static idx_t cached_n = 0;

    if (n == 0)
        return (seq_t)0;

    if (cached_n < n) {
        idx_t new_n = n;
        cache = (seq_t *)realloc(cache, (new_n + 1) * sizeof(seq_t));

        for (idx_t k = cached_n + 1; k <= new_n; ++k) {
            if (k == 1) {
                cache[k] = (seq_t)0;
            } else if (k == 2) {
                cache[k] = (seq_t)8;
            } else if (k == 3) {
                cache[k] = (seq_t)800;
            } else {
                cache[k] = (seq_t)99 * cache[k - 1] - (seq_t)99 * cache[k - 2] + cache[k - 3];
            }
        }
        cached_n = new_n;
    }
    return cache[n];
}
