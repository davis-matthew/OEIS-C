/*
 * A042296  Numerators of continued fraction convergents to sqrt(674).
 * https://oeis.org/A042296
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy
 * Formula:   a(n) = 1350*a(n-4) - a(n-8); a(0)=25, a(1)=26, a(2)=649,
 *            a(3)=675, a(4)=34399, a(5)=35074, a(6)=876175, a(7)=911249
 * Range:     u8 n <= 1, u16 n <= 5, u32 n <= 11, u64 n <= 23, u128 n <= 47,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cap   = 0;
    static const seq_t init[8] =
        { 25, 26, 649, 675, 34399, 35074, 876175, 911249 };

    if ((size_t)n + 1 > cap) {
        size_t new_cap = (size_t)n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();        /* allocation failed */
        cache = tmp;

        for (idx_t i = (idx_t)cap; i <= n; ++i) {
            if (i < 8) {
                cache[i] = init[i];
            } else {
                cache[i] = ((seq_t)1350 * cache[i-4]) - cache[i-8];
            }
        }
        cap = new_cap;
    }

    return cache[n];
}
