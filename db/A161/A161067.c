/*
 * A161067  Number of partitions of n into powers of two minus one where every
 *          part appears at least 5 times.
 * https://oeis.org/A161067
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of partitions of n into parts of the form 2^k-1
 *            (for k≥1), with the restriction that each distinct part must
 *            appear at least 5 times in the partition. The value is computed
 *            by dynamic programming over the allowed part sizes s =
 *            1,3,7,15,31,…, each requiring a minimum of 5 copies. Th...
 * Range:     u8 n <= 131, u16 n <= 499, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* Handle trivial case: for n == 0 nothing is defined in the OEIS
       offset (a(1) is the first term), but we may compute dp[0] for
       convenience. */
    if (n == (idx_t)0)
        return (seq_t)0;

    /* Convert n to a size_t for array indexing; n is small enough for
       this.  */
    const size_t N = (size_t)n;

    /* Allocate DP tables.  dp holds the computed values before the
       current part is processed; newdp will hold the updated values. */
    seq_t *dp    = (seq_t *)calloc(N + 1, sizeof(seq_t));
    seq_t *newdp = (seq_t *)calloc(N + 1, sizeof(seq_t));
    if (!dp || !newdp) {
        /* Allocation failure: return 0 (the sequence is 0 for small n) */
        free(dp);
        free(newdp);
        return (seq_t)0;
    }

    /* Initial condition: one way to partition 0 (empty partition). */
    dp[0] = (seq_t)1;

    /* Process each allowed part size `s = 2^k - 1`, requiring at least
       five copies whenever it is used.  The number of part sizes is
       roughly log_2(n), so we run a small loop. */
    for (idx_t k = 1; ; ++k) {
        /* Compute `s`, guarding against overflow; `n` is at most 10000
           in the test regime, so k ≤ 14. */
        idx_t s = ((idx_t)1 << k) - 1;
        if (s > n)
            break;

        const size_t S = (size_t)s;          /* for indexing */

        /* First copy the previous DP table into `newdp` – this accounts
           for the case of using zero copies of the current part. */
        for (size_t i = 0; i <= N; ++i)
            newdp[i] = dp[i];

        /* For each residue class modulo `s`, maintain a running sum
           of the contributions that use at least five copies of the
           current part.  The formula below implements the sum
           for `dp[amount]` with at least five copies efficiently. */
        for (size_t r = 0; r < S; ++r) {
            seq_t running = (seq_t)0;
            for (size_t a = r; a <= N; a += S) {
                size_t m = (a - r) / S;          /* number of s’s used */
                if (m >= 5)
                    running += dp[a - 5 * S];    /* add the new term */
                newdp[a] = dp[a] + running;     /* total count for `a` */
            }
        }

        /* Swap dp and newdp for the next part. */
        seq_t *tmp = dp;
        dp = newdp;
        newdp = tmp;
    }

    /* The answer for the requested `n` is now in dp[n]. */
    seq_t result = dp[N];

    /* Free temporary arrays. */
    free(dp);
    free(newdp);

    return result;
}
