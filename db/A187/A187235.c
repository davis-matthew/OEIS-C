/*
 * A187235  Number of ways to place n nonattacking semi-bishops on an n X n
 *          board.
 * https://oeis.org/A187235
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, nice
 * Formula:   a(n) = sum of products of lengths of all n‐element subsets of
 *            the * 2n−1 diagonals of an n×n board. * The lengths of the
 *            diagonals are 1,2,…,n,…,2,1. * (This is equivalent to the
 *            coefficient of x^n in ∏_{d=0}^{2n-2}(1+len[d]·x).)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 8, u64 n <= 14, u128 n <= 24,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;          /* defined only for n ≥ 1 */

    /* convert to size_t for convenient looping – n≤10000 in typical usage */
    size_t N = (size_t)n;
    size_t diag_count = 2 * N - 1;

    /* dp[k] = coefficient of x^k after processing some diagonals */
    seq_t *dp = (seq_t*)calloc(N + 1, sizeof(seq_t));
    if (!dp) return (seq_t)0;          /* allocation failed – unlikely */

    dp[0] = (seq_t)1;

    for (size_t d = 0; d < diag_count; ++d) {
        /* length of diagonal d */
        size_t len = (d < N) ? d + 1 : 2 * N - 1 - d;
        seq_t lenS = (seq_t)len;

        /* how many k can be reached after this diagonal
         * (cannot exceed N, cannot exceed d+1) */
        size_t upper = (d + 1 < N ? d + 1 : N);

        /* update in reverse to avoid overwriting needed values */
        for (size_t k = upper; k > 0; --k) {
            dp[k] += dp[k - 1] * lenS;
        }
    }

    seq_t result = dp[N];
    free(dp);
    return result;
}
