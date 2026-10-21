/*
 * A212549  Number of partitions of n containing at least one part m-9 if m is
 *          the largest part.
 * https://oeis.org/A212549
 *
 * Offset:    9    (a(9) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{i>0} p_{≤9+i}(n-(2i+9)), where p_{≤k}(x) is the number
 *            of partitions of x whose parts are ≤ k
 * Range:     u8 n <= 24, u16 n <= 50, u32 n <= 142, u64 n <= 456,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *part = NULL;      /* a(n) for 0…max_n  */
static idx_t  max_n = 0;        /* current maximum index computed */

/* Recompute all values up to N (inclusive) using a simple coin‑change DP. */
static void recompute(idx_t N)
{
    size_t M = (size_t)N;
    seq_t *dp  = (seq_t*)malloc((M + 1) * sizeof(seq_t));
    seq_t *tmp = (seq_t*)malloc((M + 1) * sizeof(seq_t));
    if (!dp || !tmp) { if (dp) free(dp); if (tmp) free(tmp); exit(1); }

    for (size_t i = 0; i <= M; ++i) {
        dp[i] = (seq_t)0;
        tmp[i] = (seq_t)0;
    }
    dp[0] = (seq_t)1;          /* partitions of 0 */

    for (size_t m = 1; m <= M; ++m) {
        /* add coin of size m */
        for (size_t k = m; k <= M; ++k) {
            dp[k] += dp[k - m];
        }
        /* add contributions to the sequence for m ≥ 10 */
        if (m >= 10) {
            size_t offset = 2 * m - 9;   /*  n - (2m-9) = n - offset */
            if (offset <= M) {
                for (size_t kk = offset; kk <= M; ++kk) {
                    tmp[kk] += dp[kk - offset];
                }
            }
        }
    }

    /* free old cache and store the new one */
    if (part) free(part);
    part = tmp;
    free(dp);

    max_n = N;
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    if (n < 9) return (seq_t)0;          /* values below the offset are 0   */
    if (n > max_n) recompute(n);         /* ensure we have values up to n   */
    return part[(size_t)n];
}
