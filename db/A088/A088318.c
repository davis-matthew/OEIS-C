/*
 * A088318  The number of possible values of the squarefree kernel (A007947)
 *          shared by at least two solutions x to A056239(x) = n.
 * https://oeis.org/A088318
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of sets S of distinct positive integers (S = prime
 *            indices in the squarefree kernel) with sum(S) <= n such that the
 *            number of partitions of n - sum(S) into parts from S is >= 2;
 *            computed by DFS over distinct-part sets with an incremental,
 *            saturating partition-count DP
 * Range:     u8 n <= 24, u16 n <= 62, u32 n <= 99, u64 n <= 99, u128 n <= 99,
 *            big any n
 * Validated: standard, pass, 69 terms through n = 96 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

static unsigned char *A088318_dp;
static size_t A088318_stride;
static unsigned long long A088318_cnt;
static int A088318_n;

static void A088318_dfs(int depth, int minpart, int s)
{
    int n = A088318_n;
    unsigned char *cur = A088318_dp + (size_t)depth * A088318_stride;
    int m;
    for (m = minpart; s + m <= n; ++m) {
        unsigned char *nx = A088318_dp + (size_t)(depth + 1) * A088318_stride;
        int ns = s + m;
        int lim = n - ns;
        int j;
        for (j = 0; j <= lim; ++j) {
            unsigned v = cur[j];
            if (j >= m) v += nx[j - m];
            nx[j] = (unsigned char)(v > 2 ? 2 : v);
        }
        if (nx[lim] >= 2) A088318_cnt++;
        A088318_dfs(depth + 1, m + 1, ns);
    }
}

seq_t a(idx_t n)
{
    int nn = (int)n;
    int maxd = 2;
    size_t rows;
    if (nn < 1) return 0;
    while (maxd * (maxd + 1) / 2 <= nn) maxd++;
    rows = (size_t)maxd + 3;
    A088318_stride = (size_t)nn + 1;
    A088318_dp = (unsigned char *)calloc(rows * A088318_stride, 1);
    if (!A088318_dp) return 0;
    A088318_dp[0] = 1;
    A088318_cnt = 0;
    A088318_n = nn;
    A088318_dfs(0, 1, 0);
    free(A088318_dp);
    A088318_dp = NULL;
    return (seq_t)A088318_cnt;
}
