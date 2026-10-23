/*
 * A117144  Partitions of n in which each part k occurs at least k times.
 * https://oeis.org/A117144
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 41, u16 n <= 125, u32 n <= 440, u64 n <= 1788,
 *            u128 n <= 8161, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A117144: Partitions of n in which each part k occurs at least k times
    Generating function:  ∏_{k≥1} (1 - x^k + x^{k^2}) / (1 - x^k)   */

/* Computes a(n) for all n ≥ 0.  The routine works in O(n √n) time and O(n)
   memory, using a dynamic programming array that records the coefficient of
   x^s after considering parts up to the current k.  For each part k the DP
   is updated by a rolling‑sum technique that accumulates the contributions
   of inserting at least k copies of that part.  The algorithm is
   independent of any external utilities.  */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* a(0) = 1 by definition */
    if (n == 0u)
        return 1;

    /* convert for array bounds; safe because n ≤ 10000 in tests */
    const size_t N = (size_t)n;

    /* DP arrays holding the current coefficients (dp) and temporary
       results after processing part k (dp2).  All entries are zero at start. */
    seq_t *dp  = (seq_t *)calloc(N + 1, sizeof(seq_t));
    seq_t *dp2 = (seq_t *)calloc(N + 1, sizeof(seq_t));
    if (!dp || !dp2) {
        free(dp);
        free(dp2);
        return 0;               /* memory allocation failed */
    }
    dp[0] = 1;                   /* empty partition */

    /* The largest part k that can possibly appear is the largest with k^2 ≤ N. */
    size_t maxk = 0;
    while ((maxk + 1) * (maxk + 1) <= N)
        ++maxk;

    for (size_t k = 1; k <= maxk; ++k) {
        /* Start with the case where part k is not used at all. */
        for (size_t i = 0; i <= N; ++i)
            dp2[i] = dp[i];

        /* Running sums for each residue class modulo k.  The sum is
           constructed by scanning s from 0 to N and adding dp[s - k^2]
           when it becomes available.  */
        seq_t *sum = (seq_t *)calloc(k, sizeof(seq_t));
        if (!sum) {
            free(dp);
            free(dp2);
            return 0;
        }

        const size_t kk = k * k;   /* minimal weight for k copies of part k */

        for (size_t s = 0; s <= N; ++s) {
            if (s >= kk) {
                size_t idx = s - kk;          /* index of earlier coefficient to add */
                sum[idx % k] += dp[idx];
            }
            dp2[s] += sum[s % k];              /* add all contributions for this residue */
        }

        free(sum);

        /* Prepare for next part: swap dp and dp2 pointers. */
        seq_t *tmp = dp;
        dp = dp2;
        dp2 = tmp;
    }

    seq_t result = dp[N];

    /* Release allocated memory. */
    free(dp);
    free(dp2);

    return result;
}
