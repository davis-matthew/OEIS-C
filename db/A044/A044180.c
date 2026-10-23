/*
 * A044180  Numbers k such that string 6,0 occurs in the base 7 representation
 *          of k but not of k-1.
 * https://oeis.org/A044180
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   The n-th integer k such that its base‑7 representation *
 *            contains the substring 60 but the base‑7 representation of k−1 *
 *            does not. (Offset 1)
 * Range:     u8 n <= 5, u16 n <= 1500, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   has60
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>
#include "../utils/has60.h"

seq_t a(idx_t n)
{
    static seq_t *cache   = NULL;   /* array of generated terms */
    static size_t cache_len = 0;    /* number of terms stored */
    static size_t cache_cap = 0;    /* current capacity */
    static idx_t  checked   = 0;   /* last integer examined */

    /* Grow the cache until it contains at least n terms. */
    while (cache_len < n)
    {
        checked++;
        seq_t k = (seq_t)checked;   /* current candidate */

        if (has60(k) && !has60(k - 1))
        {
            /* Ensure enough capacity. */
            if (cache_len == cache_cap)
            {
                size_t new_cap = (cache_cap == 0) ? 16 : cache_cap * 2;
                seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                cache = tmp;
                cache_cap = new_cap;
            }
            cache[cache_len++] = k;
        }
    }

    return cache[n - 1];
}
