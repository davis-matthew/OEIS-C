/*
 * A033675  Trajectory of 166 under map x->x + (x-with-digits-reversed).
 * https://oeis.org/A033675
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 0, u16 n <= 5, u32 n <= 16, u64 n <= 43, u128 n <= 87,
 *            big any n
 * Helpers:   grow_seq, rev_decimal
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A033675: a(0) = 166; a(n+1) = a(n) + rev_decimal(a(n)) */

#include "../utils/rev_decimal.h"
#include "../utils/grow_seq.h"

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;      /* array of a values */
    static idx_t size = 0;          /* number of terms computed */
    static idx_t cap  = 0;          /* allocated capacity of cache */

    /* Expand cache to at least n+1 elements */
    if (size <= n)
    {
        grow_seq(&cache, &cap, (size_t)(n + 1));

        for (idx_t i = size; i <= n; ++i)
        {
            if (i == 0)
                cache[0] = (seq_t)166;
            else
                cache[i] = cache[i - 1] + rev_decimal(cache[i - 1]);
        }
        size = n + 1;
    }

    return cache[n];
}
