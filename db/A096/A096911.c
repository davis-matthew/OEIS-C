/*
 * A096911  Number of partitions of n into distinct parts with exactly one
 *          even part.
 * https://oeis.org/A096911
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = Σ_{e even, 2≤e≤n} P_odd(n–e), where P_odd(m) counts
 *            partitions of m into distinct odd parts. Implementation uses
 *            dynamic programming
 * Range:     u8 n <= 41, u16 n <= 62, u32 n <= 62, u64 n <= 62, u128 n <= 62,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *dp_odd = NULL;   /* number of partitions of i into distinct odd parts */
static seq_t *a_tbl  = NULL;   /* desired sequence values a(i)   */
static idx_t  precomp_n = 0;   /* highest n for which tables are built */

static void precompute(idx_t N)
{
    if (N <= precomp_n) return;

    /* Allocate new arrays of size N+1 */
    seq_t *new_dp = (seq_t*)malloc((size_t)(N+1) * sizeof(seq_t));
    seq_t *new_a  = (seq_t*)malloc((size_t)(N+1) * sizeof(seq_t));
    if (!new_dp || !new_a) return;            /* allocation failure – should not happen */

    /* initialise to zero */
    for (idx_t i = 0; i <= N; ++i) {
        new_dp[i] = 0;
        new_a[i]  = 0;
    }
    new_dp[0] = 1;   /* one way to partition 0 */

    /* Compute partitions into distinct odd parts */
    for (idx_t coin = 1; coin <= N; coin += 2) {
        for (idx_t s = N; s >= coin; --s) {
            new_dp[s] += new_dp[s - coin];
        }
    }

    /* Compute the sequence a(n) */
    new_a[0] = 0;   /* unused, but keep defined */
    for (idx_t n = 1; n <= N; ++n) {
        seq_t sum = 0;
        for (idx_t e = 2; e <= n; e += 2) {
            sum += new_dp[n - e];
        }
        new_a[n] = sum;
    }

    /* Replace old tables with the new ones */
    free(dp_odd);
    free(a_tbl);
    dp_odd = new_dp;
    a_tbl  = new_a;
    precomp_n = N;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* out of range, not used by tests */
    precompute(n);
    return a_tbl[n];
}
