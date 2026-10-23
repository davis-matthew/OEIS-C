/*
 * A109544  Expansion of (1 + x - x^3 - 2*x^4)/(1 - x^2 - x^3 - x^4 - x^5).
 * https://oeis.org/A109544
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-2) + a(n-3) + a(n-4) + a(n-5) Initial values: a(0)=1,
 *            a(1)=1, a(2)=1, a(3)=1, a(4)=1
 * Range:     u8 n <= 15, u16 n <= 28, u32 n <= 41, u64 n <= 41, u128 n <= 41,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static state: store computed terms */
    static seq_t *dp = NULL;          /* array of computed values */
    static size_t dp_len = 0;         /* number of values currently stored (dp_len == highest index + 1) */
    static size_t dp_cap = 0;         /* current allocation size */

    /* --- initialise on first call ----------------------------------- */
    if (dp == NULL) {
        /* start with room for at least 16 terms */
        dp_cap = 16;
        dp = (seq_t *)realloc(dp, dp_cap * sizeof(seq_t));
        /* set the first five values */
        dp[0] = 1;
        dp[1] = 1;
        dp[2] = 1;
        dp[3] = 1;
        dp[4] = 1;
        dp_len = 5;    /* highest index computed so far is 4 */
    }

    /* --- extend array if needed ----------------------------------- */
    if ((size_t)n >= dp_len) {
        size_t target = (size_t)n;
        /* enlarge capacity (double until big enough) */
        size_t new_cap = dp_cap;
        while (new_cap <= target) {
            new_cap = new_cap * 2;
        }
        if (new_cap > dp_cap) {
            dp = (seq_t *)realloc(dp, new_cap * sizeof(seq_t));
            dp_cap = new_cap;
        }

        /* compute missing terms from dp_len up to target */
        for (size_t i = dp_len; i <= target; ++i) {
            /* recurrence: a(i) = a(i-2)+a(i-3)+a(i-4)+a(i-5) */
            /* the indices i-2 to i-5 are guaranteed < i and >= 0 */
            seq_t sum = dp[i-2] + dp[i-3] + dp[i-4] + dp[i-5];
            dp[i] = sum;
        }
        dp_len = target + 1;
    }

    return dp[(size_t)n];
}
