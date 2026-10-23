/*
 * A106835  Expansion of 2*x^2*(-2+9*x+3*x^2)/((2*x^2+5*x-1)*(2*x^2-5*x+1)).
 * https://oeis.org/A106835
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, less
 * Formula:   a(n) = 10*a(n-1) - 25*a(n-2) + 4*a(n-4) Initial values: a(1) = 0
 *            a(2) = 4 a(3) = 22 a(4) = 114
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 14, u64 n <= 22, u128 n <= 22,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    /* Handle indices below the offset in any way — they are irrelevant */
    if (n < 1) {
        return (seq_t)0;
    }

    /* Persistent state to cache computed values */
    static seq_t *dp = NULL;
    static idx_t dp_size = 0;
    static idx_t max_computed = 0;

    /* Ensure capacity for the required index (dp is 1‑based) */
    if (dp_size <= n) {
        idx_t new_size = (n + 8);          /* a little slack to reduce reallocs */
        seq_t *tmp = (seq_t *)realloc(dp, new_size * sizeof(seq_t));
        if (!tmp) return (seq_t)0;         /* out‑of‑memory; unlikely in tests */
        dp = tmp;
        dp_size = new_size;
        /* Initialise newly allocated entries to zero (safe for unsigned types) */
        for (idx_t i = max_computed + 1; i < dp_size; ++i) {
            dp[i] = (seq_t)0;
        }
    }

    /* Set base cases if they are inside the requested range */
    if (n >= 1 && max_computed < 1) dp[1] = (seq_t)0;
    if (n >= 2 && max_computed < 2) dp[2] = (seq_t)4;
    if (n >= 3 && max_computed < 3) dp[3] = (seq_t)22;
    if (n >= 4 && max_computed < 4) dp[4] = (seq_t)114;

    /* Compute any missing values up to n */
    for (idx_t i = max_computed + 1; i <= n; ++i) {
        if (i <= 4) {
            /* base values already set; skip computation */
            continue;
        }
        dp[i] = (seq_t)((seq_t)10 * dp[i-1]
                       - (seq_t)25 * dp[i-2]
                       + (seq_t)4  * dp[i-4]);
    }

    max_computed = n > max_computed ? n : max_computed;
    return dp[n];
}
