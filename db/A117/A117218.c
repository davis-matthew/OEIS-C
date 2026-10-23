/*
 * A117218  Squares divisible by their number of digits.
 * https://oeis.org/A117218
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   squares divisible by their number of digits
 * Range:     u8 n <= 9, u16 n <= 80, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   digit_len
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/digit_len.h"

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;
    static size_t cache_len = 0;
    static idx_t next_k = 0;

    while (cache_len < n) {
        seq_t square = (seq_t)next_k * (seq_t)next_k;
        int d = digit_len(square);
        if (square % (seq_t)d == 0) {
            if (cache_len + 1 > cache_cap) {
                size_t new_cap = cache_cap ? cache_cap * 2 : 8;
                seq_t *new_arr = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
                if (!new_arr) abort();
                cache = new_arr;
                cache_cap = new_cap;
            }
            cache[cache_len++] = square;
        }
        next_k++;
    }

    return cache[n-1];
}
