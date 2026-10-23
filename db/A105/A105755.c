/*
 * A105755  Lucas 9-step numbers.
 * https://oeis.org/A105755
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1) + a(n-2) + … + a(n-9) for n>0, a(0) = 9, a(-1) = …
 *            = a(-8) = -1
 * Range:     u8 n <= 8, u16 n <= 16, u32 n <= 32, u64 n <= 64, u128 n <= 128,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Memoization buffer: a[i] stores a(i) */
    static seq_t *dp = NULL;
    static idx_t dp_size = 0;      /* number of computed elements (dp[0..dp_size-1]) */

    /* Ensure the buffer exists */
    if (dp == NULL) {
        /* Allocate space for at least one element (a(0)) */
        dp = (seq_t*)malloc(sizeof(seq_t));
        dp_size = 0;
    }

    /* Grow buffer if necessary */
    if (n + 1 > dp_size) {
        idx_t new_size = n + 1;
        dp = (seq_t*)realloc(dp, new_size * sizeof(seq_t));

        for (idx_t i = dp_size; i <= n; ++i) {
            if (i == 0) {
                dp[0] = (seq_t)9;
            } else {
                seq_t sum = 0;
                /* Number of terms that would refer to negative indices */
                idx_t missing = (i < 9) ? (9 - i) : 0;

                /* Add existing previous terms */
                for (idx_t k = 1; k <= 9 && i >= k; ++k) {
                    sum += dp[i - k];
                }
                /* Subtract the missing terms (each contributes -1) */
                sum -= missing;   /* missing ≤ 8, safe for unsigned types */

                dp[i] = sum;
            }
        }
        dp_size = new_size;
    }

    return dp[n];
}
