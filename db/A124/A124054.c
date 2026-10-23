/*
 * A124054  Array(d,n) = number of ordered ways to write n as the sum of d
 *          squares less than d, read by rows, through last nonzero value per
 *          row.
 * https://oeis.org/A124054
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, tabf
 * Formula:   not stated
 * Range:     u8 n <= 75, u16 n <= 75, u32 n <= 75, u64 n <= 75, u128 n <= 75,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) is obtained by locating the row d whose list of
 * counts (for ordered d‑tuples of numbers < d whose squares sum to
 * a given value) contains the n‑th term, and returning that count.
 * The algorithm enumerates rows, computing the number of
 * representations for each possible sum via dynamic programming
 * until the target position is reached.
 */

#include <stdlib.h>
#include <string.h>

seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;   /* offset is 1 */

    idx_t cum = 0;   /* total terms seen so far */

    seq_t *dp   = NULL; /* current DP array */
    seq_t *next = NULL; /* temporary DP array */

    /* Find the row d that contains the n-th term. */
    for (idx_t d = 1; ; ++d)
    {
        idx_t max_sum = (d - 1) * (d - 1) * d;       /* d·(d‑1)² */
        size_t sz    = (size_t)max_sum + 1;          /* array size */

        /* Allocate temporary arrays for this row. */
        dp   = (seq_t *)malloc(sz * sizeof(seq_t));
        next = (seq_t *)malloc(sz * sizeof(seq_t));
        if (!dp || !next)
        {
            free(dp); free(next);
            return (seq_t)0;
        }

        /* Initialise dp[0] = 1, others zero. */
        memset(dp, 0, sz * sizeof(seq_t));
        dp[0] = (seq_t)1;

        /* Dynamic programming: add each component one by one. */
        for (idx_t comp = 0; comp < d; ++comp)
        {
            memset(next, 0, sz * sizeof(seq_t));

            for (idx_t s = 0; s <= max_sum; ++s)
            {
                seq_t val = dp[s];
                if (val == (seq_t)0) continue;

                for (idx_t v = 0; v < d; ++v)
                {
                    idx_t w = (idx_t)v * v;      /* square of v */
                    idx_t t = s + w;
                    if (t > max_sum) break;
                    next[t] += val;
                }
            }

            /* Swap dp and next for next iteration. */
            seq_t *tmp = dp; dp = next; next = tmp;
        }

        /* Determine the length of this row (last non‑zero entry + 1). */
        idx_t last_nonzero = max_sum;
        while (last_nonzero > 0 && dp[last_nonzero] == (seq_t)0)
            --last_nonzero;
        idx_t row_len = last_nonzero + 1;

        /* Does the desired index lie in this row? */
        if (cum + row_len >= n)
            break;

        /* Move to the next row. */
        cum += row_len;
        free(dp);
        free(next);
    }

    /* At this point: dp holds the counts for the target row d. */
    idx_t pos = n - cum - 1;          /* 0‑based position in the row */
    seq_t result = dp[pos];

    free(dp);
    free(next);
    return result;
}
