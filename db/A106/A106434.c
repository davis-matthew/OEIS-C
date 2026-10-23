/*
 * A106434  The (1,1)-entry of the matrix A^n, where A = [0,1;2,3].
 * https://oeis.org/A106434
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, less
 * Formula:   a(1)=0, a(2)=2, a(n)=3*a(n-1)+2*a(n-2) for n≥3
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 19, u64 n <= 36, u128 n <= 71,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *dp = NULL;          /* memoization table 1‑based */
    static idx_t dp_size = 0;         /* largest n computed so far */

    /* Grow the table if needed */
    if (n > dp_size) {
        size_t new_cap = (size_t)(n + 1); /* indices 0..n, 0 unused */
        seq_t *tmp = (seq_t *)realloc(dp, new_cap * sizeof(seq_t));
        if (!tmp) { /* allocation failed – return 0 as a safe fallback */
            return 0;
        }
        dp = tmp;

        /* Initialise new entries */
        for (idx_t i = dp_size + 1; i <= n; ++i) {
            if (i == 1) {
                dp[i] = (seq_t)0;
            } else if (i == 2) {
                dp[i] = (seq_t)2;
            } else {
                dp[i] = 3 * dp[i - 1] + 2 * dp[i - 2];
            }
        }
        dp_size = n;
    }
    return dp[n];
}
