/*
 * A029047  Expansion of 1/((1-x)*(1-x^3)*(1-x^6)*(1-x^10)).
 * https://oeis.org/A029047
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = number of partitions of n into the parts 1, 3, 6 and 10
 * Range:     u8 n <= 55, u16 n <= 403, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    /* static cache to avoid recomputing the same values */
    static seq_t *dp = NULL;
    static idx_t dp_max = 0;          /* largest n for which dp is valid */

    /* if we have not computed up to n yet, recompute */
    if (dp == NULL || n > dp_max) {
        /* allocate a new array larger enough for indices up to n */
        seq_t *newdp = (seq_t *)malloc((n + 1) * sizeof(seq_t));
        if (newdp == NULL) { /* allocation failure – return 0 */
            return 0;
        }

        /* initialise to zero and set the base case */
        for (idx_t i = 0; i <= n; ++i) {
            newdp[i] = 0;
        }
        newdp[0] = 1;   /* one way to make 0: use none of the parts */

        /* denominations in ascending order */
        const idx_t coins[4] = {1, 3, 6, 10};

        /* classic coin‑change DP: for each coin, update all reachable sums */
        for (int ci = 0; ci < 4; ++ci) {
            idx_t c = coins[ci];
            for (idx_t i = c; i <= n; ++i) {
                newdp[i] += newdp[i - c];
            }
        }

        /* free old cache and store the new one */
        if (dp != NULL) free(dp);
        dp = newdp;
        dp_max = n;
    }

    return dp[n];
}
