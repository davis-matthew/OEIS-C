/*
 * A108153  a(n) = 3*a(n-1) + 2*a(n-2) + a(n-3).
 * https://oeis.org/A108153
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3*a(n-1) + 2*a(n-2) + a(n-3) for n ≥ 3, with initial
 *            values a(0)=0, a(1)=1, a(2)=3
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 18, u64 n <= 26, u128 n <= 26,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>   /* for size_t */

seq_t a(idx_t n)
{
    /* a fixed-size cache large enough for all expected queries (up to ~10000) */
    static seq_t dp[20001];
    static size_t dp_last = 2;   /* largest index computed so far */
    static int init = 0;         /* flag indicating first use */

    /* initialise the cache on first use */
    if (!init) {
        dp[0] = (seq_t)0;
        dp[1] = (seq_t)1;
        dp[2] = (seq_t)3;
        init = 1;
    }

    /* if the requested value has already been computed, return it */
    if ((size_t)n <= dp_last) {
        return dp[(size_t)n];
    }

    /* compute missing terms up to n using the recurrence */
    size_t i;
    for (i = dp_last + 1; i <= (size_t)n; ++i) {
        dp[i] = (seq_t)3 * dp[i - 1] + (seq_t)2 * dp[i - 2] + dp[i - 3];
    }
    dp_last = (size_t)n;
    return dp[(size_t)n];
}
