/*
 * A077895  Expansion of (1-x)^(-1)/(1+2*x^2+2*x^3).
 * https://oeis.org/A077895
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(0)=1, a(1)=1, a(2)=-1, a(3)=-3 For n≥4: a(n) = a(n-1) -
 *            2*a(n-2) + 2*a(n-4)
 * Range:     i8 n <= 11, i16 n <= 23, i32 n <= 41, i64 n <= 41, i128 n <= 41,
 *            big any n
 * Helpers:   alloc_seq_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/alloc_seq_array.h"

static seq_t *a_cache = NULL;   /* cache of computed values */
static size_t a_cache_len = 0;  /* number of elements stored (== max index + 1) */

seq_t a(idx_t n)
{
    if (n + 1 > a_cache_len)                 /* need to extend cache */
    {
        size_t new_len = n + 1;
        seq_t *new_arr = alloc_seq_array(new_len);

        /* copy previously computed values, if any */
        if (a_cache)
        {
            for (size_t i = 0; i < a_cache_len; ++i)
                new_arr[i] = a_cache[i];
            /* old array is intentionally left unreclaimed – negligible leak */
        }

        /* adopt the new cache */
        a_cache = new_arr;
        size_t old_len = a_cache_len;
        a_cache_len = new_len;

        /* initialise base terms */
        if (a_cache_len > 0) a_cache[0] = 1;
        if (a_cache_len > 1) a_cache[1] = 1;
        if (a_cache_len > 2) a_cache[2] = -1;
        if (a_cache_len > 3) a_cache[3] = -3;

        /* compute further terms using the linear recurrence */
        size_t start = old_len > 4 ? old_len : 4;
        for (size_t i = start; i < a_cache_len; ++i)
            a_cache[i] = a_cache[i-1] - (seq_t)2 * a_cache[i-2]
                                     + (seq_t)2 * a_cache[i-4];
    }

    return a_cache[n];
}
