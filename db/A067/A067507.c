/*
 * A067507  Powers of 2 with even digit sum.
 * https://oeis.org/A067507
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   a(n) = the n-th power of 2 whose decimal digit sum is even (n ≥
 *            1)
 * Range:     u8 n <= 4, u16 n <= 9, u32 n <= 12, u64 n <= 22, u128 n <= 22,
 *            big any n
 * Helpers:   digit_sum
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/digit_sum.h"

seq_t a(idx_t n) {
    if (n == 0) return 0;          /* offset is 1, undefined for n==0 */

    /* cached terms of the sequence */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;
    static size_t cache_cap = 0;

    /* most recently computed power of 2 */
    static seq_t last_value = 1;   /* 2⁰ */

    /* grow the cache if necessary */
    if (cache_len == cache_cap) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 16;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return 0;            /* allocation failed */
        cache      = tmp;
        cache_cap  = new_cap;
    }

    /* generate terms until we have at least n of them */
    while (cache_len < n) {
        /* compute next power of 2 (last_value = 2^k) */
        last_value *= 2;

        /* check parity of digit sum */
        if ((digit_sum(last_value) & 1u) == 0) {
            cache[cache_len++] = last_value;
        }
    }

    return cache[n - 1];
}
