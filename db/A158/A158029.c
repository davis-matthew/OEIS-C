/*
 * A158029  Sides of squares which are filled exactly (no holes, no overlaps)
 *          by the digits needed to write a subsequence of consecutive square
 *          numbers, starting with 0.
 * https://oeis.org/A158029
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) is the nth integer s for which the cumulative number of
 *            decimal digits of 0², 1², …, k² equals s². Computed by an
 *            incremental search
 * Range:     u8 n <= 4, u16 n <= 12, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   digit_len
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>
#include "../utils/digit_len.h"

seq_t a(idx_t n)
{
    /* static state */
    static seq_t *res = NULL;      /* array of found square‐roots */
    static size_t  res_len = 0;    /* number of values stored */
    static size_t  res_cap = 0;    /* array capacity */
    static idx_t   last_i  = 0;    /* next i to process (square root index) */
    static seq_t   sum     = 0;    /* cumulative digit count */
    static seq_t   m       = 0;    /* floor(sqrt(sum)) */

    /* extend the sequence until we have at least n terms */
    while (res_len < n)
    {
        /* compute digits of i² */
        seq_t sq   = (seq_t)last_i * (seq_t)last_i;
        int   digs = digit_len(sq);

        /* update sum of digits */
        sum += (seq_t)digs;

        /* increase m while (m+1)² ≤ sum */
        while ((m + (seq_t)1) * (m + (seq_t)1) <= sum)
            ++m;

        /* record if sum is a perfect square */
        if (m * m == sum)
        {
            if (res_len == res_cap)
            {
                size_t new_cap = res_cap ? res_cap * 2 : 64;
                seq_t *tmp = (seq_t *)realloc(res, new_cap * sizeof(seq_t));
                if (!tmp) abort();       /* allocation failed */
                res = tmp;
                res_cap = new_cap;
            }
            res[res_len++] = m;
        }

        ++last_i;   /* next index */
    }

    return res[n - 1];
}
