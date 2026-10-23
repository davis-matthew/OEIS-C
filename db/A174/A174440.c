/*
 * A174440  Partial sums of A022544.
 * https://oeis.org/A174440
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 14, u16 n <= 50, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   not_sum_of_two_squares
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: a(n) = SUM_{i=1..n} A022544(i)
   where A022544 contains integers that are NOT representable as a sum of
   two squares (i.e. they have some prime p ≡ 3 (mod 4) raised to an odd
   exponent). This function builds the sequence lazily and memoises the
   partial sums so that arbitrary orders of calls are fast. */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/not_sum_of_two_squares.h"

/* External linkage function required by the test harness. */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;   /* not used by OEIS offset, but harmless */

    /* static state */
    static seq_t *cum = NULL;      /* cumulative sums */
    static idx_t cap = 0;          /* capacity of cum array */
    static idx_t last_num = 2;     /* last integer examined */
    static idx_t cache_n = 0;      /* number of terms already cached */

    /* grow capacity if necessary */
    if (n > cap)
    {
        size_t new_cap = (cap == 0) ? 64 : cap * 2;
        while (new_cap < (size_t)n)
            new_cap *= 2;
        seq_t *tmp = (seq_t *)realloc(cum, new_cap * sizeof(seq_t));
        if (!tmp)
        {
            /* allocation failure: abort (not expected in tests) */
            abort();
        }
        cum = tmp;
        cap = new_cap;
    }

    /* generate terms up to n */
    for (idx_t i = cache_n; i < n; ++i)
    {
        /* find next integer > last_num that satisfies the condition */
        do { ++last_num; } while (!not_sum_of_two_squares(last_num));

        /* store cumulative sum */
        if (i == 0)
            cum[i] = (seq_t)last_num;
        else
            cum[i] = cum[i - 1] + (seq_t)last_num;
    }

    cache_n = n;
    return cum[n - 1];
}
