/*
 * A036024  Number of partitions of n into parts not of form 4k+2, 20k, 20k+1
 *          or 20k-1. Also number of partitions in which no odd part is
 *          repeated, with no part of size less than or equal to 2 and where
 *          differences between parts at distance 4 are greater than 1 when
 *          the smallest part is odd and greater than...
 * https://oeis.org/A036024
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 33, u16 n <= 57, u32 n <= 57, u64 n <= 57, u128 n <= 57,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Restricted-part partition numbers.
   a(n) = number of partitions of n into parts not of the form
   4k+2, 20k, 20k+1 or 20k-1.  The sequence is OEIS A036024.  */
#include <stdbool.h>
#include <stdlib.h>

static seq_t *dp = NULL;   /* dp[i] = number of admissible partitions of i */
static idx_t dp_cap = 0;   /* highest index for which dp has been filled */

/* Return true iff the integer p is allowed as a part. */
static bool allowed_part(idx_t p)
{
    if (p % 4 == 2)     /* 4k+2 */
        return false;
    idx_t mod20 = p % 20;
    if (mod20 == 0 || mod20 == 1 || mod20 == 19) /* 20k, 20k+1, 20k-1 */
        return false;
    return true;
}

/* Compute a(n).  The function memoises results and only recomputes
   the partition table when a larger n is requested. */
seq_t a(idx_t n)
{
    if (n == 0)          /* offset is 1; behaviour for n<1 is undefined */
        return 0;

    if (n > dp_cap) {
        /* Allocate a new table up to n.  The old one will be discarded. */
        seq_t *new_dp = (seq_t *)malloc((n + 1) * sizeof(seq_t));
        if (!new_dp)
            return 0;    /* allocation failed – return 0 as a safe fallback */

        /* Reset counts; dp[0] = 1 represents the empty partition. */
        new_dp[0] = 1;
        for (idx_t i = 1; i <= n; ++i)
            new_dp[i] = 0;

        /* Classic coin‑change dynamic programming.  For each admissible
           part p we add its contribution to all sums >= p. */
        for (idx_t p = 1; p <= n; ++p) {
            if (!allowed_part(p))
                continue;
            for (idx_t s = p; s <= n; ++s) {
                new_dp[s] += new_dp[s - p];
            }
        }

        /* Replace the old table.  No need to free new_dp itself. */
        free(dp);
        dp = new_dp;
        dp_cap = n;
    }

    return dp[n];
}
