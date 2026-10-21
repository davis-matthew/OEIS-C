/*
 * A086346  On a 3 X 3 board, the number of n-move paths for a chess king
 *          ending in a given corner square.
 * https://oeis.org/A086346
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0) = 1, a(1) = 3, a(2) = 18 a(n) = 2*a(n-1) + 12*a(n-2) +
 *            8*a(n-3) for n ≥ 3
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 14, u64 n <= 28, u128 n <= 56,
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
        seq_t *tmp = (seq_t*)realloc(cache, new_size * sizeof(seq_t));
        if (!tmp) abort();                 /* out of memory */
        cache = tmp;

        for (idx_t i = cache_size; i < new_size; ++i) {
            if (i == 0)
                cache[i] = (seq_t)1;
            else if (i == 1)
                cache[i] = (seq_t)3;
            else if (i == 2)
                cache[i] = (seq_t)18;
            else
                cache[i] = cache[i-1] * (seq_t)2
                         + cache[i-2] * (seq_t)12
                         + cache[i-3] * (seq_t)8;
        }
        cache_size = new_size;
    }
    return cache[n];
}
