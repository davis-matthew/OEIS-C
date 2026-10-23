/*
 * A011578  Stirling numbers of second kind S2(29,n).
 * https://oeis.org/A011578
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 1, u32 n <= 2, u64 n <= 5, u128 n <= 29,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Stirling numbers of the second kind S(29,n) */
/* a(n) = S(29,n)  for 1 ≤ n ≤ 29, otherwise 0 */

#include <stdbool.h>

static bool initialized = false;
static seq_t cache[30] = {0};

static void compute_row29(void)
{
    seq_t prev[30] = {0};
    seq_t curr[30] = {0};

    prev[0] = (seq_t)1;                 /* S(0,0) = 1 */

    for (idx_t n = 1; n <= 29; ++n)
    {
        curr[0] = (seq_t)0;
        for (idx_t k = 1; k <= n; ++k)
        {
            curr[k] = (seq_t)k * prev[k] + prev[k - 1];
        }
        /* copy current row to previous */
        for (idx_t k = 0; k <= n; ++k)
        {
            prev[k] = curr[k];
        }
    }

    /* store the final row S(29,1..29) */
    for (idx_t k = 1; k <= 29; ++k)
    {
        cache[k] = prev[k];
    }
}

seq_t a(idx_t n)
{
    if (!initialized)
    {
        compute_row29();
        initialized = true;
    }
    if (n > 29 || n < 1)
        return (seq_t)0;
    return cache[n];
}
