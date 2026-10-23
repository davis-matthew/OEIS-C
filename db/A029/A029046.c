/*
 * A029046  Expansion of 1/((1-x)*(1-x^3)*(1-x^6)*(1-x^8)).
 * https://oeis.org/A029046
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 51, u16 n <= 52, u32 n <= 52, u64 n <= 52, u128 n <= 52,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A029046: Number of partitions of n into parts 1, 3, 6 and 8
   We compute it by dynamic programming on the generating function
   1/((1‑x)*(1‑x³)*(1‑x⁶)*(1‑x⁸)).  For each part p in {1,3,6,8} we
   update dp[i] += dp[i‑p] for all i ≥ p.  This algorithm is O(k·n)
   and uses only unsigned arithmetic, so it works for arbitrary‑precision
   seq_t as well as for 8–128 bit unsigned types. */

#include <stdlib.h>

static seq_t *dp = NULL;   /* memoised table of partitions for 0..max_n */
static idx_t dp_size = 0;  /* current number of allocated entries */

seq_t a(idx_t n)
{
    /* If the table is too small, rebuild it for 0..n */
    if (!dp || dp_size <= n) {
        idx_t newsize = n + 1;
        seq_t *newdp = (seq_t *)malloc(newsize * sizeof(seq_t));
        if (!newdp) return 0; /* allocation failed – return 0 */
        /* initialise */
        for (idx_t i = 0; i < newsize; ++i) newdp[i] = 0;
        newdp[0] = 1;
        /* parts allowed in the partitions */
        const int parts[] = {1, 3, 6, 8};
        const size_t n_parts = sizeof parts / sizeof parts[0];
        for (size_t p = 0; p < n_parts; ++p) {
            idx_t part = (idx_t)parts[p];
            for (idx_t i = part; i < newsize; ++i) {
                newdp[i] += newdp[i - part];
            }
        }
        /* replace old table */
        free(dp);
        dp = newdp;
        dp_size = newsize;
    }
    return dp[n];
}
