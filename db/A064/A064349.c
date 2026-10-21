/*
 * A064349  Generating function: 1/((1-x)*(1-x^2)^2*(1-x^3)^3*(1-x^4)^4).
 * https://oeis.org/A064349
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the coefficient of xⁿ in 1/((1−x)(1−x²)²(1−x³)³(1−x⁴)⁴)
 * Range:     u8 n <= 10, u16 n <= 30, u32 n <= 137, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation: a dynamic‑programming coin‑change algorithm using   */
/* the ten “colors” 1, 2, 2, 3, 3, 3, 4, 4, 4, 4 corresponding to the  */
/* 1, 2, 3, 4 part sizes each having k independent kinds.          */

#include <stdlib.h>   /* malloc, free, abort */
#include <stddef.h>   /* size_t */

/* Persistent DP table.  table[0] = 1, table[i] = a(i).          */
static seq_t *dp = NULL;          /* array of coefficients               */
static size_t dp_size = 0;        /* number of allocated entries (>=1)   */

/* Return a(n).  The table is rebuilt on demand.                  */
seq_t a(idx_t n)
{
    /* If we already have the value, just return it.            */
    if (dp != NULL && n < dp_size)
        return dp[n];

    /* Allocate a new table large enough for all indices 0…n.  */
    size_t new_size = (size_t)(n + 1);
    seq_t *new_dp = (seq_t *)malloc(new_size * sizeof(seq_t));
    if (!new_dp) abort();

    /* Initialise the table. */
    new_dp[0] = (seq_t)1;
    /* Remaining entries start at 0; we can leave them undefined
       during the DP pass because they will be written before read. */
    for (idx_t i = 1; i <= n; ++i)
        new_dp[i] = (seq_t)0;

    /* Unbounded‑coin DP: process each “color” separately.       */
    /* Part size 1 – 1 copy.                                     */
    for (idx_t s = 1; s <= n; ++s)
        new_dp[s] += new_dp[s - 1];

    /* Part size 2 – 2 copies.                                   */
    for (idx_t copy = 0; copy < 2; ++copy)
        for (idx_t s = 2; s <= n; ++s)
            new_dp[s] += new_dp[s - 2];

    /* Part size 3 – 3 copies.                                   */
    for (idx_t copy = 0; copy < 3; ++copy)
        for (idx_t s = 3; s <= n; ++s)
            new_dp[s] += new_dp[s - 3];

    /* Part size 4 – 4 copies.                                   */
    for (idx_t copy = 0; copy < 4; ++copy)
        for (idx_t s = 4; s <= n; ++s)
            new_dp[s] += new_dp[s - 4];

    /* Replace the old table.                                    */
    if (dp != NULL) free(dp);
    dp     = new_dp;
    dp_size = new_size;

    return dp[n];
}
