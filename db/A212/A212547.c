/*
 * A212547  Number of partitions of n containing at least one part m-7 if m is
 *          the largest part.
 * https://oeis.org/A212547
 *
 * Offset:    7    (a(7) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 22, u16 n <= 49, u32 n <= 144, u64 n <= 458,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A212547 – Number of partitions of n containing at least one part m‑7
    if m is the largest part.
    Formula (OEIS):  a(n) = Σ_{i≥1} P_{7+i}(n – 2 i – 7)
    where P_k(m) is the number of partitions of m with all parts ≤ k.
    Generating function:  Σ_{i>0} x^{2i+7} / Π_{j=1}^{7+i} (1–x^j).  */

#include <stdlib.h>
#include <string.h>

seq_t a(idx_t n)
{
    /* Only defined for n ≥ 7; return 0 for smaller indices. */
    if (n < 7) return 0;

    /* Static tables that keep the computation between calls. */
    static seq_t *dp   = NULL;   /* partition numbers with parts ≤ current m */
    static seq_t *ans  = NULL;   /* final answer array */
    static idx_t  size = 0;      /* number of elements stored (≥ computed n+1) */

    /* Grow the tables if the requested n is larger than what we have. */
    if (n >= size) {
        idx_t new_size = n + 1;                  /* indices 0 … n */
        seq_t *new_dp  = (seq_t *)malloc(new_size * sizeof(seq_t));
        seq_t *new_ans = (seq_t *)malloc(new_size * sizeof(seq_t));
        if (!new_dp || !new_ans) {               /* allocation failed */
            free(new_dp);
            free(new_ans);
            return 0;
        }
        memset(new_dp,  0, new_size * sizeof(seq_t));
        memset(new_ans, 0, new_size * sizeof(seq_t));
        new_dp[0] = 1;                           /* empty partition */

        /* Build the tables up to n. */
        for (idx_t m = 1; m <= n; ++m) {
            /* Classical partition DP: add part of size m. */
            for (idx_t k = m; k <= n; ++k) {
                new_dp[k] += new_dp[k - m];
            }

            /* For m = 7 + i (i ≥ 1) we must add contributions to
               the answer array.  The mandatory parts m and m‑7 add
               a shifted weight of 2 m − 7. */
            if (m >= 8) {
                idx_t shift = 2 * m - 7;
                if (shift > n) continue;
                for (idx_t t = 0; t + shift <= n; ++t) {
                    new_ans[shift + t] += new_dp[t];
                }
            }
        }

        /* Replace old tables with the new ones. */
        free(dp);
        free(ans);
        dp   = new_dp;
        ans  = new_ans;
        size = new_size;
    }

    return ans[n];
}
