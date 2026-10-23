/*
 * A036005  Number of partitions of n into parts not of the form 25k, 25k+6 or
 *          25k-6. Also number of partitions with at most 5 parts of size 1
 *          and differences between parts at distance 11 are greater than 1.
 * https://oeis.org/A036005
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = number of partitions of n into parts not of the form *
 *            25k, 25k+6, or 25k-6 (i.e., parts ≢ 0, 6, 19 (mod 25))
 * Range:     u8 n <= 17, u16 n <= 46, u32 n <= 46, u64 n <= 46, u128 n <= 46,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Standard headers; no custom utilities required. */
#include <stdlib.h>

/*  Static cache of partition counts up to the largest n seen. */
static seq_t *dp = NULL;     /* array of dp values */
static size_t dp_size = 0;   /* allocated size of dp */

/*  Return the desired partition count for the given n.  */
seq_t a(idx_t n)
{
    /* The sequence is defined for n >= 1; returning 0 for n == 0 is harmless. */
    if (n == (idx_t)0) {
        return (seq_t)0;
    }

    /* Ensure the DP array is large enough.  If a larger n is requested,
       recompute the whole table from scratch.  */
    if ((size_t)n + 1U > dp_size) {
        /* Allocate a new array of the required size. */
        seq_t *newdp = (seq_t *)malloc(sizeof(seq_t) * ((size_t)n + 1U));
        if (newdp == NULL) {
            /* Allocation failed – return 0 (unlikely for n up to 10 000). */
            return (seq_t)0;
        }
        /* Free any previous array. */
        if (dp != NULL) {
            free(dp);
        }
        dp     = newdp;
        dp_size = (size_t)n + 1U;

        /* Initialise dp[0] = 1 and others to 0. */
        dp[0] = (seq_t)1;
        for (size_t i = 1U; i < dp_size; ++i) {
            dp[i] = (seq_t)0;
        }

        /* Dynamic‑programming over all allowed part sizes. */
        for (idx_t part = (idx_t)1; part <= n; ++part) {
            /* Disallowed residues modulo 25: 0, 6, 19. */
            idx_t r = (idx_t)(part % (idx_t)25);
            if (r == (idx_t)0 || r == (idx_t)6 || r == (idx_t)19) {
                continue;
            }

            /* Add contributions of this part to all sums >= part. */
            for (size_t i = (size_t)part; i <= (size_t)n; ++i) {
                dp[i] += dp[i - (size_t)part];
            }
        }
    }

    /* Now dp[n] contains the required value. */
    return dp[(size_t)n];
}
