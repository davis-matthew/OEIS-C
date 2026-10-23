/*
 * A035994  Number of partitions of n into parts not of the form 23k, 23k+6 or
 *          23k-6. Also number of partitions with at most 5 parts of size 1
 *          and differences between parts at distance 10 are greater than 1.
 * https://oeis.org/A035994
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 17, u16 n <= 46, u32 n <= 46, u64 n <= 46, u128 n <= 46,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A035994
    a(n) = number of partitions of n into parts whose residue
           modulo 23 is not 0, 6, or 17.  */
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

static seq_t *dp = NULL;          /* DP table for partition counts  */
static idx_t dp_size = 0;         /* allocated size of dp (>= n+1) */

/* Return true if part p is allowed (not congruent to 0,6,17 mod 23). */
static inline bool allowed_part(idx_t p)
{
    int r = (int)(p % 23);
    return r != 0 && r != 6 && r != 17;
}

/* Return the n‑th term of the sequence.  */
seq_t a(idx_t n)
{
    if (n == 0) {
        /* Not used for the sequence but avoids accessing dp[-1] */
        return (seq_t)1;
    }

    /* Ensure dp has room for indices 0..n.  */
    if (n + 1 > dp_size) {
        seq_t *new_dp = (seq_t *)realloc(dp, sizeof(seq_t) * (n + 1));
        if (new_dp == NULL) {
            /* Allocation failure – return 0.  */
            return (seq_t)0;
        }
        dp      = new_dp;
        dp_size = n + 1;
    }

    /* Recompute all values using allowed parts up to n.  */
    for (idx_t i = 0; i <= n; ++i) {
        dp[i] = (seq_t)0;
    }
    dp[0] = (seq_t)1;

    for (idx_t p = 1; p <= n; ++p) {
        if (!allowed_part(p))
            continue;
        for (idx_t i = p; i <= n; ++i) {
            dp[i] += dp[i - p];
        }
    }

    return dp[n];
}
