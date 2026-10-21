/*
 * A064985  Number of partitions of n into factorial parts ( 0! allowed ).
 * https://oeis.org/A064985
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = number of partitions of n into factorial parts (0! is
 *            allowed as a distinct part type). This is computed by an
 *            unbounded knapsack DP over the factorial values: a(0)=1,
 *            a(s)=Σ_{f in facts, f≤s} a(s-f)
 * Range:     u8 n <= 21, u16 n <= 129, u32 n <= 1679, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Allocate DP array */
    seq_t *dp = (seq_t*)calloc((size_t)(n + 1), sizeof(seq_t));
    if (!dp) return 0;                /* out of memory – unlikely in tests */
    dp[0] = 1;

    /* Build list of factorials up to n.
       0! and 1! are both 1, and are treated as distinct types.  */
    idx_t facts[32];      /* few enough for all n≤10 000 */
    size_t fcnt = 0;

    facts[fcnt++] = 1;    /* 0! = 1 */
    idx_t fact = 1;       /* will become 1! = 1 next */
    for (idx_t i = 1; ; ++i) {
        fact *= i;                    /* i! */
        if (fact > n) break;
        facts[fcnt++] = fact;         /* include this factorial */
    }

    /* Unbounded knapsack DP: process each factorial type once. */
    for (size_t pi = 0; pi < fcnt; ++pi) {
        idx_t f = facts[pi];
        for (idx_t s = f; s <= n; ++s) {
            dp[s] += dp[s - f];
        }
    }

    seq_t result = dp[n];
    free(dp);
    return result;
}
