/*
 * A230338  Recurrence equation: a(0) = 1 and a(n) = a(n-1)*sqrt(21*a(n-1)^2 +
 *          4) for n >= 1.
 * https://oeis.org/A230338
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0) = 1, a(1) = 5, a(n) = a(n-1)*(21*a(n-2)^2 + 2) for n >= 2
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 3, u64 n <= 4, u128 n <= 5,
 *            big any n
 * Helpers:   ensure_capacity
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/ensure_capacity.h"

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cap   = 0;
    static idx_t stored = 0;

    if (n >= stored)
    {
        idx_t i;
        for (i = stored; i <= n; ++i)
        {
            /* Ensure the array has room for i+1 elements (0‑based)   */
            ensure_capacity(i + 1, &cap, (void **)&cache, sizeof(seq_t));

            if (i == 0)
            {
                cache[0] = 1;
            }
            else if (i == 1)
            {
                cache[1] = 5;
            }
            else
            {
                seq_t prev  = cache[i - 1];
                seq_t prev2 = cache[i - 2];
                seq_t m = 21;
                m = m * prev2 * prev2;   /* 21 * prev2^2 */
                m = m + 2;               /* add 2   */
                cache[i] = prev * m;     /* multiply by previous term */
            }
        }
        stored = n + 1;
    }

    return cache[n];
}
