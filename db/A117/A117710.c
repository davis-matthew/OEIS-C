/*
 * A117710  Pentagonal numbers for which the product of the digits is also a
 *          pentagonal number.
 * https://oeis.org/A117710
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = the nth pentagonal number such that the product of its
 *            decimal digits is also a pentagonal number
 * Range:     u8 n <= 1, u16 n <= 18, u32 n <= 42, u64 n <= 42, u128 n <= 42,
 *            big any n
 * Helpers:   digit_product, floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>
#include "../utils/digit_product.h"
#include "../utils/floor_sqrt.h"

static bool is_pentagonal(seq_t x)
{
    if (x == (seq_t)0)
        return true;
    seq_t t2 = (seq_t)24 * x + (seq_t)1;
    seq_t t  = floor_sqrt(t2);
    if (t * t != t2)
        return false;
    return ((t + (seq_t)1) % (seq_t)6) == (seq_t)0;
}

seq_t a(idx_t n)
{
    static seq_t *cache     = NULL;
    static size_t cache_sz  = 0;
    static size_t cache_cap = 0;
    static idx_t  m_next    = 0;   /* next index m for generating pentagonal numbers */

    while (cache_sz <= n)
    {
        seq_t pent;
        if (m_next == 0)
        {
            pent = (seq_t)0;
        }
        else
        {
            seq_t m  = (seq_t)m_next;
            seq_t num = m * ((seq_t)3 * m - (seq_t)1);
            pent = num / (seq_t)2;
        }

        seq_t prod = digit_product(pent);
        if (is_pentagonal(prod))
        {
            if (cache_sz == cache_cap)
            {
                size_t new_cap = cache_cap ? cache_cap * 2 : 16;
                seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                if (!tmp)
                    abort(); /* memory allocation failed */
                cache = tmp;
                cache_cap = new_cap;
            }
            cache[cache_sz++] = pent;
        }
        m_next++;
    }
    return cache[n];
}
