/*
 * A218291  Multiples of 6 such that the sum of their digits is also a
 *          multiple of 6.
 * https://oeis.org/A218291
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) is the nth multiple of 6 whose digit sum is also a multiple
 *            of 6. This implementation performs a lazy brute‑force search
 * Range:     u8 n <= 20, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   digit_sum
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>
#include <stdbool.h>
#include "../utils/digit_sum.h"

seq_t a(idx_t n)
{
    /* static cache of already found terms */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;
    static size_t cache_size = 0;
    /* next multiple index to test; a(1) corresponds to 6*1 */
    static idx_t next_k = 1;

    /* generate terms until we have at least n of them */
    while (cache_size < n)
    {
        seq_t num = (seq_t)6 * (seq_t)next_k;

        /* check digit‑sum condition */
        if ((digit_sum(num) % 6) == 0)
        {
            if (cache_size + 1 > cache_cap)
            {
                size_t new_cap = (cache_cap == 0) ? 8 : cache_cap << 1;
                seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                cache = tmp;
                cache_cap = new_cap;
            }
            cache[cache_size++] = num;
        }
        ++next_k;
    }

    return cache[n - 1];
}
