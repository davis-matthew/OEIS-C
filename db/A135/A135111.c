/*
 * A135111  Numbers such that the digital sum base 2 and the digital sum base
 *          3 are in a ratio of 2:3.
 * https://oeis.org/A135111
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = the n-th positive integer m (n≥1) such that * 3 *
 *            digit_sum_base2(m) == 2 * digit_sum_base3(m). * The function
 *            remembers the values it has found so far to answer * subsequent
 *            calls efficiently
 * Range:     u8 n <= 18, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   digit_sum_base2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include "../utils/digit_sum_base2.h"

static uint64_t digit_sum_base3(seq_t x)
{
    uint64_t s = 0;
    while (x != 0)
    {
        /* remainder of a big‑int modulo an int fits in an unsigned long long */
        s += (unsigned long long)(x % 3);
        x /= 3;
    }
    return s;
}

seq_t a(idx_t n)
{
    /* static cache */
    static idx_t  computed   = 0;      /* how many terms have been found       */
    static seq_t *cache      = NULL;   /* array of found terms                */
    static size_t cap        = 0;      /* current allocation capacity         */
    static seq_t  last_checked = 0;    /* last integer examined               */

    /* ensure there is room for at least one new entry before the loop */
    if (computed + 1 > cap)
    {
        size_t newcap = cap == 0 ? 4 : cap << 1;
        cache = (seq_t *)realloc(cache, newcap * sizeof(seq_t));
        cap   = newcap;
    }

    /* grow the cache until we have at least n terms */
    while (computed < n)
    {
        /* next candidate integer */
        ++last_checked;
        seq_t m = last_checked;

        uint64_t s2 = digit_sum_base2(m);
        uint64_t s3 = digit_sum_base3(m);

        if (3ULL * s2 == 2ULL * s3)
        {
            /* store the term */
            cache[computed++] = m;

            /* grow cache if needed for future terms */
            if (computed + 1 > cap)
            {
                size_t newcap = cap == 0 ? 4 : cap << 1;
                cache = (seq_t *)realloc(cache, newcap * sizeof(seq_t));
                cap   = newcap;
            }
        }
    }

    return cache[(size_t)(n - 1)];
}
