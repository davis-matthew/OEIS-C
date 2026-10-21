/*
 * A080013  Number of permutations satisfying -k<=p(i)-i<=r and p(i)-i not in
 *          I, i=1..n, with k=2, r=2, I={0,1}.
 * https://oeis.org/A080013
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-2)+a(n-3)+a(n-4)-a(n-6) with a(0)=1, a(1)=0, a(2)=0,
 *            a(3)=1, a(4)=1, a(5)=1
 * Range:     u8 n <= 21, u16 n <= 37, u32 n <= 70, u64 n <= 136,
 *            u128 n <= 267, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* initial terms for n = 0..5 */
    static const seq_t base[6] = {1, 0, 0, 1, 1, 1};

    /* memoised table of already computed values */
    static seq_t *dp = NULL;
    static idx_t dp_size = 0;

    /* small n handled directly */
    if (n < 6)
        return base[n];

    /* first time: allocate enough space and fill everything up to n */
    if (dp_size == 0) {
        dp_size = n + 1;
        dp = (seq_t*)malloc(dp_size * sizeof(seq_t));
        if (!dp) exit(1);          /* allocation failed */
        for (idx_t i = 0; i < 6; ++i) dp[i] = base[i];
        for (idx_t i = 6; i <= n; ++i)
            dp[i] = dp[i-2] + dp[i-3] + dp[i-4] - dp[i-6];
        return dp[n];
    }

    /* enlarge the table if the requested index is outside the current limits */
    if (n >= dp_size) {
        idx_t newsize = n + 1;
        seq_t *tmp = (seq_t*)realloc(dp, newsize * sizeof(seq_t));
        if (!tmp) exit(1);          /* allocation failed */
        dp = tmp;
        for (idx_t i = dp_size; i <= n; ++i)
            dp[i] = dp[i-2] + dp[i-3] + dp[i-4] - dp[i-6];
        dp_size = newsize;
    }

    /* dp[n] is now valid */
    return dp[n];
}
