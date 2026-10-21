/*
 * A221638  Numbers that are divisible by the sum of the squares of their
 *          digits when alternately added and subtracted starting from units
 *          (rightmost).
 * https://oeis.org/A221638
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 34, u16 n <= 1500, u32 n <= 1500, u64 n <= 1500,
 *            u128 n <= 1500, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search: a(n) is the n-th positive integer divisible by the alternating
 * sum of the squares of its digits (units digit first). */
#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cache_len = 0;
    static size_t cache_cap = 0;
    static idx_t next_candidate = 1;

    while (cache_len < n) {
        seq_t cand = (seq_t)next_candidate;
        seq_t x = cand;
        int alt = 0;
        int sign = 1;

        while (x > 0) {
            seq_t digit = x % 10;
            int d = (int)digit;
            int dsq = d * d;
            alt += sign > 0 ? dsq : -dsq;
            sign = -sign;
            x /= 10;
        }

        if (alt != 0) {
            int alt_abs = alt > 0 ? alt : -alt;
            if (cand % (seq_t)alt_abs == 0) {
                if (cache_len == cache_cap) {
                    size_t new_cap = cache_cap ? cache_cap * 2 : 16;
                    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                    cache = tmp;
                    cache_cap = new_cap;
                }
                cache[cache_len++] = cand;
            }
        }
        next_candidate++;
    }

    return cache[n - 1];
}
