/*
 * A043522  Numbers having two 8's in base 10.
 * https://oeis.org/A043522
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 3056, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute the nth term of OEIS A043522: numbers having exactly two 8's in base 10.
   There is no simple formula, so a straightforward search with caching is used. */
#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache     = NULL;
    static size_t cache_len = 0;
    static size_t cache_cap = 0;
    static seq_t last_val = (seq_t)87;   /* 87 just before first term 88 */

    while (cache_len < n) {
        seq_t candidate = last_val + (seq_t)1;
        seq_t t = candidate;
        unsigned eight_cnt = 0;

        /* Count all digits, but stop early if more than 2 8's */
        while (t != (seq_t)0) {
            seq_t digit = t % (seq_t)10;
            if (digit == (seq_t)8) {
                ++eight_cnt;
                if (eight_cnt > 2) break;   /* can't be a term */
            }
            t = t / (seq_t)10;
        }

        if (eight_cnt == 2) {
            if (cache_len == cache_cap) {
                size_t new_cap = cache_cap ? cache_cap * 2 : 16;
                seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                if (!tmp) abort();
                cache     = tmp;
                cache_cap = new_cap;
            }
            cache[cache_len++] = candidate;
        }

        last_val = candidate;
    }

    return cache[n - 1];
}
