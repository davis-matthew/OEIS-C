/*
 * A161296  Number of partitions of n into numbers not divisible by 4 where
 *          every part appears at least 5 times.
 * https://oeis.org/A161296
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 59, u16 n <= 148, u32 n <= 425, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = #partitions of n into parts not divisible by 4
 *               where each used part appears at least 5 times.
 * Computation: dynamic programming over part sizes.
 * For each admissible part m (m % 4 != 0) we consider the generating
 * factor 1 + x^{5m} + x^{6m} + … .
 * The DP keeps an array dp[s] = number of partitions of the processed
 * part sizes achieving sum s.  For a new part m we copy dp to next
 * (representing the choice “don’t use m”) and add, for every current
 * sum t, contributions to all sums t + 5m + k·m (k ≥ 0).  After processing
 * all part sizes, dp[n] is the desired value.
 */

#include <stddef.h>
#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* n is 1‑based */
    if (n == 0) return 0;

    size_t N = (size_t)n;      /* for array indexing */
    size_t i, t, s;
    seq_t *dp, *next;

    /* Allocate dp and next arrays. */
#ifdef __cplusplus
    dp   = new seq_t[N + 1];
    next = new seq_t[N + 1];
#else
    dp   = (seq_t *)malloc((N + 1) * sizeof(seq_t));
    next = (seq_t *)malloc((N + 1) * sizeof(seq_t));
#endif
    if (!dp || !next) {
#ifdef __cplusplus
        delete[] dp; delete[] next;
#else
        free(dp); free(next);
#endif
        return 0;   /* memory allocation failed – return 0 */
    }

    /* Initialize dp[0] = 1 (empty partition) and others 0. */
    for (i = 0; i <= N; ++i) dp[i] = 0;
    dp[0] = 1;

    /* Process all part sizes m that are not divisible by 4. */
    for (i = 1; i <= N; ++i) {
        size_t m = i;
        if (m % 4 == 0) continue;          /* skip multiples of 4 */

        /* If 5*m > N, this part can never be used. */
        if (5 * m > N) continue;

        /* next starts as a copy of dp (option “don’t use m”). */
        for (t = 0; t <= N; ++t) next[t] = dp[t];

        /* For every current sum t add contributions with at least
         * five copies of the part m. */
        size_t max_t = N - 5 * m;           /* largest t that can be extended */
        for (t = 0; t <= max_t; ++t) {
            seq_t val = dp[t];
            if (!val) continue;
            size_t s_start = t + 5 * m;
            for (s = s_start; s <= N; s += m) {
                next[s] = next[s] + val;
            }
        }

        /* Swap dp and next for the next iteration. */
#ifdef __cplusplus
        seq_t *tmp = dp;
        dp = next;
        next = tmp;
#else
        seq_t *tmp = dp;
        dp = next;
        next = tmp;
#endif
    }

    seq_t result = dp[N];

    /* Free allocated memory. */
#ifdef __cplusplus
    delete[] dp;
    delete[] next;
#else
    free(dp);
    free(next);
#endif

    return result;
}
