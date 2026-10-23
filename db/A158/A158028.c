/*
 * A158028  Square numbers s such that all the digits needed to write the
 *          consecutive square numbers from 0 to s fill exactly a square (no
 *          holes, no overlaps).
 * https://oeis.org/A158028
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 10, u32 n <= 28, u64 n <= 28, u128 n <= 28,
 *            big any n
 * Helpers:   digit_len, isqrt_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A158028: squares s for which the cumulative number of digits of all squares from 0^2 up to s^2 is a perfect square. */
#include <stddef.h>
#include <stdlib.h>
#include "../utils/digit_len.h"
#include "../utils/isqrt_seq.h"

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cache_len = 0;
    static size_t cache_cap = 0;
    static idx_t last_k = 0;   /* last processed k */
    static seq_t csum = 0;     /* cumulative digit sum */

    while (cache_len < (size_t)n) {
        idx_t k = last_k;
        seq_t k_sq = (seq_t)k * (seq_t)k;
        int dlen = digit_len(k_sq);
        csum += (seq_t)dlen;
        seq_t r = isqrt_seq(csum);
        if (r * r == csum) {
            if (cache_len + 1 > cache_cap) {
                size_t new_cap = (cache_cap == 0) ? 1 : cache_cap * 2;
                seq_t *new_arr = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                if (!new_arr) {
                    /* allocation failed; in test environments this should not happen */
                    return 0;
                }
                cache = new_arr;
                cache_cap = new_cap;
            }
            cache[cache_len++] = k_sq;
        }
        last_k++;
    }
    return cache[n - 1];
}
