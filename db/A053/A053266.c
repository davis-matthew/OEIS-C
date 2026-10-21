/*
 * A053266  Coefficients of the '5th-order' mock theta function Phi(q).
 * https://oeis.org/A053266
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = coefficient of q^n in * -1 + Σ_{k≥0} q^{5k^2} / (
 *            ∏_{j=1..k} (1-q^{5j-1})(1-q^{5j+1}) ) * (the product is empty
 *            for k = 0, so the k = 0 term equals 1/(1-q)). * This
 *            implementation precomputes a(n) up to the required index using *
 *            dynamic programming on partition numbers. It stores all comp...
 * Range:     u8 n <= 60, u16 n <= 170, u32 n <= 539, u64 n <= 1857,
 *            u128 n <= 6778, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *memo = NULL;      /* array of computed a(n) values       */
static idx_t memo_len = 0;      /* length of memo (number of entries) */

static void precompute(idx_t N)
{
    /* Allocate new memo and DP arrays */
    seq_t *new_a = (seq_t *)malloc((N + 1) * sizeof(seq_t));
    seq_t *dp   = (seq_t *)malloc((N + 1) * sizeof(seq_t));

    /* Initial values for k = 0: partitions using only part 1 */
    dp[0] = 1;
    new_a[0] = 1;
    for (idx_t i = 1; i <= N; ++i) {
        dp[i] = 1;
        new_a[i] = 1;
    }

    /* Process k = 1,2,... while 5k^2 ≤ N */
    for (idx_t k = 1; 5 * k * k <= N; ++k) {
        idx_t part1 = 5 * k - 1;   /* new part of size 5k-1 */
        idx_t part2 = 5 * k + 1;   /* new part of size 5k+1 */

        /* Add partitions involving part1 */
        if (part1 <= N) {
            for (idx_t i = part1; i <= N; ++i) {
                dp[i] += dp[i - part1];
            }
        }

        /* Add partitions involving part2 */
        if (part2 <= N) {
            for (idx_t i = part2; i <= N; ++i) {
                dp[i] += dp[i - part2];
            }
        }

        /* Shift contributions for this k (q^{5k^2}) */
        idx_t shift = 5 * k * k;
        for (idx_t i = shift; i <= N; ++i) {
            new_a[i] += dp[i - shift];
        }
    }

    /* Subtract the initial -1 constant term from a(0) */
    new_a[0] -= 1;

    /* Replace old memo with the newly computed array */
    if (memo != NULL) free(memo);
    memo = new_a;
    memo_len = N + 1;

    free(dp);
}

seq_t a(idx_t n)
{
    if (n >= memo_len) precompute(n);
    return memo[n];
}
