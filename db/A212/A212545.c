/*
 * A212545  Number of partitions of n containing at least one part m-5 if m is
 *          the largest part.
 * https://oeis.org/A212545
 *
 * Offset:    5    (a(5) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of partitions of n that contain a part m-5, where
 *            m is the largest part. Generating function: ∑_{i>0} x^(2*i+5) /
 *            ∏_{j=1}^{5+i} (1-x^j). The implementation computes all values up
 *            to the largest n requested by dynamic programming on restricted
 *            partition numbers
 * Range:     u8 n <= 21, u16 n <= 50, u32 n <= 146, u64 n <= 460,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>   /* calloc, free */

static idx_t computed_max = 0;   /* largest n for which tables are filled */
static seq_t *f = NULL;          /* partial partition counts (parts ≤ current m) */
static seq_t *ans = NULL;        /* final sequence values */

seq_t a(idx_t n)
{
    if (n < 5) return (seq_t)0;    /* offset is 5, earlier values unused */

    if (n > computed_max) {
        /* Recompute tables up to n (inclusive) */
        seq_t *new_f = (seq_t*)calloc(n + 1, sizeof(seq_t));
        seq_t *new_ans = (seq_t*)calloc(n + 1, sizeof(seq_t));
        if (!new_f || !new_ans) {  /* allocation failure: return 0 */
            free(new_f);
            free(new_ans);
            return (seq_t)0;
        }

        /* f[0] = 1 represents the empty partition */
        new_f[0] = (seq_t)1;

        for (idx_t m = 1; m <= n; ++m) {
            /* Update partitions with parts up to m */
            for (idx_t k = m; k <= n; ++k) {
                new_f[k] += new_f[k - m];
            }

            if (m >= 6) {
                idx_t offset = 2 * m - 5;
                if (offset > n) continue;        /* no contribution beyond n */

                /* Add contributions of this largest part m to ans */
                for (idx_t k = 0; k <= n - offset; ++k) {
                    new_ans[k + offset] += new_f[k];
                }
            }
        }

        /* Replace old tables with the new ones */
        free(f);
        free(ans);
        f = new_f;
        ans = new_ans;
        computed_max = n;
    }

    return ans[n];
}
