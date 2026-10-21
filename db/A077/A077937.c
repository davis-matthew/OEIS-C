/*
 * A077937  Expansion of 1/(1-2*x-2*x^2+2*x^3).
 * https://oeis.org/A077937
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*a(n-1) + 2*a(n-2) - 2*a(n-3); a(0)=1, a(1)=2, a(2)=6
 * Range:     u8 n <= 6, u16 n <= 12, u32 n <= 24, u64 n <= 48, u128 n <= 97,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;   /* number of terms currently stored */

    if (n >= cache_size) {
        idx_t new_size = n + 1;
        cache = (seq_t*)realloc(cache, (size_t)new_size * sizeof(seq_t));

        /* compute any missing terms */
        for (idx_t i = cache_size; i <= n; ++i) {
            if (i == 0) {
                cache[0] = (seq_t)1;
            } else if (i == 1) {
                cache[1] = (seq_t)2;
            } else if (i == 2) {
                cache[2] = (seq_t)6;
            } else {
                cache[i] = (seq_t)2 * cache[i-1]
                         + (seq_t)2 * cache[i-2]
                         - (seq_t)2 * cache[i-3];
            }
        }
        cache_size = new_size;
    }
    return cache[n];
}
