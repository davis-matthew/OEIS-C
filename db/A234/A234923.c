/*
 * A234923  Array w(n,h), in which row n shows the weights, as defined in
 *          Comments, of the distinct-parts partitions of n, arranged in
 *          Mathematica order.
 * https://oeis.org/A234923
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, tabf
 * Formula:   not stated
 * Range:     u8 n <= 68, u16 n <= 68, u32 n <= 68, u64 n <= 68, u128 n <= 68,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes A234923: concatenation of the weights of all
   distinct‑part partitions of n in Mathematica order. */
#include <stdint.h>
#include <stddef.h>

/* idx_t and seq_t are supplied by the build. */

/* constants for precomputation */
#define MAX_SUM 200

static idx_t dp[MAX_SUM + 1][MAX_SUM + 1];
static idx_t p_d[MAX_SUM + 1];
static idx_t cum[MAX_SUM + 1];
static int initialized = 0;

/* precompute dp, p_d and cum once */
static void init(void)
{
    int s, j;
    for (j = 0; j <= MAX_SUM; ++j)
        dp[0][j] = 1;          /* one way to partition zero */

    for (s = 1; s <= MAX_SUM; ++s)
        dp[s][0] = 0;          /* cannot partition positive with parts <=0 */

    for (s = 1; s <= MAX_SUM; ++s) {
        for (j = 1; j <= MAX_SUM; ++j) {
            if (j <= s)
                dp[s][j] = dp[s][j - 1] + dp[s - j][j - 1];
            else
                dp[s][j] = dp[s][j - 1];
        }
    }
    cum[0] = 0;
    for (s = 1; s <= MAX_SUM; ++s) {
        p_d[s] = dp[s][s];          /* number of distinct partitions of s */
        cum[s] = cum[s - 1] + p_d[s];
    }
    initialized = 1;
}

/* recursively find the r‑th distinct partition of n with parts <= max_part
   in Mathematica (lexicographic decreasing) order.  idx `pos` is the current
   length of the partition array. */
static void find_partition(int n, int max_part, idx_t r,
                           int *parts, int *pos)
{
    if (n == 0) return;

    int kmax = (n < max_part) ? n : max_part;
    for (int k = kmax; k >= 1; --k) {
        idx_t cnt = dp[n - k][k - 1];
        if (r > cnt) {
            r -= cnt;
            continue;
        } else {
            parts[(*pos)++] = k;
            find_partition(n - k, k - 1, r, parts, pos);
            return;
        }
    }
}

seq_t a(idx_t n)
{
    if (!initialized) init();

    /* find integer m such that cum[m-1] < n <= cum[m] */
    int m = 1;
    while (m <= MAX_SUM && cum[m] < n)
        ++m;

    /* index of partition within the list for m */
    idx_t r = n - cum[m - 1];

    /* build the r‑th partition of m */
    int parts[MAX_SUM];
    int pos = 0;
    find_partition(m, m, r, parts, &pos);

    /* compute weight: sum_{i=0}^{len-1} (len-i)*parts[i] */
    seq_t weight = 0;
    for (int i = 0; i < pos; ++i) {
        weight += (seq_t)(pos - i) * (seq_t)parts[i];
    }
    return weight;
}
