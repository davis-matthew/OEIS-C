/*
 * A206678  Number of (n+1) X 2 0..2 arrays with every 2 X 3 or 3 X 2 subblock
 *          having an unequal number of clockwise and counterclockwise edge
 *          increases.
 * https://oeis.org/A206678
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   * a(1) = 81, a(2) = 306, a(3) = 1404, a(4) = 5352 * For n ≥ 5:
 *            a(n) = 16·a(n‑2) + 4·a(n‑3) + 8·a(n‑4) * This is the empirical
 *            recurrence found for OEIS A206678
 * Range:     u8 n <= 1, u16 n <= 5, u32 n <= 13, u64 n <= 29, u128 n <= 60,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Accept only indices >= 1; 0 is undefined but we return 0 for safety. */
    if (n == 0)
        return 0;

    /* Cache of previously computed terms.  dp[0] holds a(1). */
    static seq_t *dp   = NULL;   /* dynamic array of computed terms   */
    static idx_t dp_sz = 0;      /* current size of dp  (number of terms) */

    /* Initialise cache with base terms if needed. */
    if (dp_sz == 0) {
        idx_t init_sz = 4;                  /* a(1)..a(4) */
        dp = (seq_t *)calloc(init_sz, sizeof(seq_t));
        if (!dp) return 0;                  /* allocation failure: return 0 */
        dp[0] = (seq_t)81;   /* a(1) */
        dp[1] = (seq_t)306;  /* a(2) */
        dp[2] = (seq_t)1404; /* a(3) */
        dp[3] = (seq_t)5352; /* a(4) */
        dp_sz = init_sz;
    }

    /* Expand cache to at least n terms. */
    if (dp_sz < n) {
        idx_t new_sz = n;
        seq_t *tmp = (seq_t *)realloc(dp, new_sz * sizeof(seq_t));
        if (!tmp) return 0; /* allocation failure */
        dp = tmp;
        /* Compute new terms using the recurrence. */
        for (idx_t i = dp_sz; i < n; ++i) {     /* i is 0‑based index for a(i+1) */
            seq_t t2 = dp[i - 2] * (seq_t)16;   /* a(i-1) → a(n‑2) */
            seq_t t3 = dp[i - 3] * (seq_t)4;    /* a(i-2) → a(n‑3) */
            seq_t t4 = dp[i - 4] * (seq_t)8;    /* a(i-3) → a(n‑4) */
            dp[i] = t2 + t3 + t4;
        }
        dp_sz = new_sz;
    }

    return dp[n - 1];  /* return a(n) */
}
