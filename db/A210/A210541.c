/*
 * A210541  Number of arrays of n nonnegative integers with value i>0
 *          appearing only after i-1 has appeared at least 4 times.
 * https://oeis.org/A210541
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 if n <= 4 a(n) = sum_{i=0}^{n-4} binomial(n-4, i) *
 *            a(i) for n > 4 Initial values: a(0) = 1, a(1) = a(2) = a(3) =
 *            a(4) = 1
 * Range:     u8 n <= 11, u16 n <= 16, u32 n <= 25, u64 n <= 39, u128 n <= 62,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>   /* for malloc, realloc */

seq_t a(idx_t n)
{
    static seq_t *dp    = NULL;   /* memoization array, 1‑based + a(0) */
    static idx_t  cap   = 0;      /* current capacity of dp   */
    static idx_t  done  = 0;      /* largest index computed so far */

    /* Allocate/expand the memoization array if necessary */
    if (n >= cap) {
        size_t new_cap = (size_t)n + 1;          /* include index 0 */
        seq_t *tmp = (seq_t*)realloc(dp, new_cap * sizeof(seq_t));
        if (!tmp) exit(1);                      /* allocation failed */
        dp  = tmp;
        cap = new_cap;
    }

    /* initialise a(0) on the first call */
    if (done == 0 && cap > 0)
        dp[0] = (seq_t)1;

    /* compute all missing values up to n */
    for (idx_t i = done + 1; i <= n; ++i) {
        if (i <= 4) {
            dp[i] = (seq_t)1;
        } else {
            idx_t k   = i - 4;            /* n-4 for this i */
            seq_t sum = (seq_t)0;
            seq_t bin = (seq_t)1;          /* binomial coefficient C(k,0) */

            for (idx_t j = 0; j <= k; ++j) {
                sum += bin * dp[j];
                if (j < k) {               /* update to C(k, j+1) */
                    bin = (bin * (seq_t)(k - j)) / (seq_t)(j + 1);
                }
            }
            dp[i] = sum;
        }
    }

    done = (done > n) ? done : n;
    return dp[n];
}
