/*
 * A013745  a(n) = 9^(3*n + 2).
 * https://oeis.org/A013745
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 9^(3*n + 2) (a(0)=81, a(n)=729*a(n-1))
 * Range:     u8 n <= 0, u16 n <= 1, u32 n <= 2, u64 n <= 6, u128 n <= 12,
 *            big any n
 * Validated: standard, pass, 27 terms through n = 200 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/mem.h"

seq_t a(idx_t n)
{
    static seq_t *memo = NULL;
    static idx_t memo_cap = 0;

    /* Ensure we have storage up to n */
    if (n + 1 > memo_cap)
    {
        idx_t new_cap = n + 1;
        if (memo == NULL)
            memo = mem_new(seq_t, new_cap);
        else
            memo = mem_resize(seq_t, memo, new_cap);

        /* Initialize the first element if this is the first allocation */
        if (memo_cap == 0)
        {
            memo[0] = (seq_t)81;      /* 9^2 = 81 */
            memo_cap = 1;
        }

        /* Fill in any missing terms */
        for (idx_t i = memo_cap; i <= n; ++i)
            memo[i] = memo[i - 1] * (seq_t)729;  /* 9^3 = 729 */

        memo_cap = new_cap;
    }

    return memo[n];
}
