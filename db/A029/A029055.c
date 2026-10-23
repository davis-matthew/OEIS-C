/*
 * A029055  Expansion of 1/((1-x)*(1-x^3)*(1-x^8)*(1-x^10)).
 * https://oeis.org/A029055
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 60, u16 n <= 444, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A029055: number of partitions of n into parts 1, 3, 8 and 10.
   A standard one‑dimensional dynamic programming with incremental
   allocation.  The table `dp` stores the values for all indices up to
   the largest n requested so far.  The implementation works both
   in C11 (malloc/delete free) and C++17 (new/delete).  For arbitrary‑
   precision builds the type `seq_t` is a class that is default‑constructible
   and supports the arithmetic operators needed here. */

#include <stdlib.h>

static seq_t *dp = NULL;
static idx_t  dp_size = 0;

/* Return the nth term of A029055. */
seq_t a(idx_t n)
{
    /* Grow the table if the requested index is beyond what we have. */
    if (n + 1 > dp_size) {
        idx_t newsize = n + 1;
#ifdef __cplusplus
        seq_t *newdp = new seq_t[newsize];
#else
        seq_t *newdp = (seq_t *)malloc(newsize * sizeof(seq_t));
#endif
        if (!newdp) return (seq_t)0;          /* allocation failed */

        /* Initially zero the new table. */
        for (idx_t i = 0; i < newsize; ++i)
            newdp[i] = 0;

        /* Base case: one way to partition 0. */
        newdp[0] = 1;

        /* Dynamic programming over parts 1, 3, 8 and 10. */
        const idx_t parts[4] = {1, 3, 8, 10};
        for (int p_i = 0; p_i < 4; ++p_i) {
            idx_t part = parts[p_i];
            for (idx_t i = part; i < newsize; ++i) {
                newdp[i] = newdp[i] + newdp[i - part];
            }
        }

        /* Free the old table. */
        if (dp) {
#ifdef __cplusplus
            delete[] dp;
#else
            free(dp);
#endif
        }

        dp     = newdp;
        dp_size = newsize;
    }

    return dp[n];
}
