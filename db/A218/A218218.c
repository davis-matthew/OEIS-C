/*
 * A218218  Table read downwards by antidiagonals: T(n,k) is the number of
 *          partitions of k into n distinct positive parts, k, n >= 0.
 * https://oeis.org/A218218
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 299, u16 n <= 299, u32 n <= 299, u64 n <= 299,
 *            u128 n <= 299, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A218218:
 * a(n) is the number of partitions of k into n distinct positive parts,
 * where (n,k) are obtained by reading the table T(n,k) along antidiagonals.
 * The implementation maps the index n to a pair (i,j) and uses a DP table
 * to count partitions into distinct parts. */

#include <stdbool.h>
#include <stddef.h>

#define MAX_D 210  /* maximum diagonal needed for n <= 10000   */
static seq_t dp[MAX_D + 1][MAX_D + 1];
static bool dp_ready = false;

/* precompute DP for subsets of {1..MAX_D} */
static void init_dp(void)
{
    if (dp_ready)
        return;
    for (size_t i = 0; i <= MAX_D; ++i)
        for (size_t j = 0; j <= MAX_D; ++j)
            dp[i][j] = 0;
    dp[0][0] = 1;
    for (int v = 1; v <= MAX_D; ++v) {
        for (int n = MAX_D; n >= 1; --n) {
            for (int k = MAX_D; k >= v; --k) {
                dp[n][k] += dp[n - 1][k - v];
            }
        }
    }
    dp_ready = true;
}

/* convert index n to pair (i,j) via antidiagonal enumeration */
static void index_to_pair(idx_t r, idx_t *i, idx_t *j)
{
    idx_t d = 0;
    idx_t cum = 1;                /* cumulative count up to current d   */
    while (cum <= r) {
        ++d;
        cum = (d + 1) * (d + 2) / 2;   /* (d+1)(d+2)/2   */
    }
    idx_t prev = d * (d + 1) / 2;       /* cumulative up to d-1   */
    idx_t offset = r - prev;
    *i = offset;
    *j = d - offset;
}

/* public function required by the build */
seq_t a(idx_t n)
{
    idx_t i, j;
    index_to_pair(n, &i, &j);
    if (i > MAX_D || j > MAX_D)
        return 0;
    init_dp();
    return dp[i][j];
}
