/*
 * A067510  Powers of 6 with digit sum divisible by 6.
 * https://oeis.org/A067510
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 6, u64 n <= 14, u128 n <= 25,
 *            big any n
 * Helpers:   digit_sum
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation: a(n) = the n-th power of 6 whose digit sum is congruent to 0 mod 6. */

#include <stdlib.h>
#include "../utils/digit_sum.h"

seq_t a(idx_t n)
{
    /* Static state for memoization and incremental construction */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;
    static size_t cache_cap = 0;
    static idx_t current_exp = 0;   /* exponent of current_power */
    static seq_t current_power = (seq_t)1;  /* 6^current_exp */

    if (n == 0)
        return (seq_t)0;  /* undefined for n<1, but return 0 safely */

    /* Expand cache until we have at least n terms */
    while (cache_len < n) {
        /* Advance to next exponent */
        ++current_exp;
        current_power *= (seq_t)6;

        /* Check digit sum divisibility by 6 */
        seq_t ds = digit_sum(current_power);
        if ((ds % 6) == 0) {
            /* Ensure buffer capacity */
            if (cache_len >= cache_cap) {
                size_t new_cap = cache_cap ? cache_cap * 2 : 8;
                seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                if (!tmp) {
                    abort();
                }
                cache = tmp;
                cache_cap = new_cap;
            }
            cache[cache_len++] = current_power;
        }
    }

    return cache[n - 1];
}
