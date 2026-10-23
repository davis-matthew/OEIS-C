/*
 * A184711  Number of strings of numbers x(i=1..11) in 0..n with sum i*x(i)
 *          equal to n*11.
 * https://oeis.org/A184711
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of length‑11 strings x[1..11] with 0≤x[i]≤n such
 *            that Σ i·x[i] = 11·n
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 20, u64 n <= 129, u128 n <= 129,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>

seq_t a(idx_t n)
{
    /* maximum sum S = 11*n */
    idx_t S = n * (idx_t)11;
    size_t len = (size_t)S + 1;

    /* dp holds counts for current weight, next for next iteration */
    seq_t *dp = (seq_t *)malloc(len * sizeof(seq_t));
    seq_t *next = (seq_t *)malloc(len * sizeof(seq_t));
    if (!dp || !next) {
        /* allocation failure: return 0 as safe fallback */
        free(dp);
        free(next);
        return (seq_t)0;
    }

    memset(dp, 0, len * sizeof(seq_t));
    memset(next, 0, len * sizeof(seq_t));
    dp[0] = (seq_t)1;   /* one way to reach sum 0 with no variables */

    for (idx_t weight = 1; weight <= 11; ++weight) {
        /* reset next array */
        memset(next, 0, len * sizeof(seq_t));

        /* sliding window size: (n+1)*weight */
        idx_t window = (n + (idx_t)1) * weight;

        for (idx_t r = 0; r < weight; ++r) {
            seq_t sum = (seq_t)0;
            for (idx_t j = r; j <= S; j += weight) {
                sum += dp[j];
                if (j >= window) {
                    sum -= dp[j - window];
                }
                next[j] = sum;
            }
        }
        /* swap dp and next for next weight */
        seq_t *tmp = dp;
        dp = next;
        next = tmp;
    }

    seq_t result = dp[S];

    free(dp);
    free(next);
    return result;
}
