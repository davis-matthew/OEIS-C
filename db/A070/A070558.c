/*
 * A070558  Number of two-rowed partitions of length 5.
 * https://oeis.org/A070558
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = coefficient of x^n in * 1 / ( (1-x) *
 *            ((1-x^2)(1-x^3)(1-x^4)(1-x^5))^2 * (1-x^6) )
 * Range:     u8 n <= 11, u16 n <= 33, u32 n <= 40, u64 n <= 40, u128 n <= 40,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Return the obvious base case first */
    if (n == 0) {
        return (seq_t)1;
    }

    /* Allocate two work arrays of length n+1 */
    size_t len = (size_t)n + 1;
    seq_t *dp   = (seq_t *)calloc(len, sizeof(seq_t));
    seq_t *next = (seq_t *)calloc(len, sizeof(seq_t));
    if (!dp || !next) {
        free(dp);
        free(next);
        return (seq_t)0;       /* allocation failed */
    }

    /* Start with the constant term 1 */
    dp[0] = (seq_t)1;

    /* Convolution helper for a factor (1 - x^k)^-2.
       For each i: next[i] = Σ_{t=0}^{⌊i/k⌋} dp[i - t*k] * (t+1). */
    #define CONVOLVE_K(k)                                           \
        for (idx_t i = 0; i <= n; ++i) {                            \
            seq_t sum = (seq_t)0;                                   \
            idx_t max_t = i / (k);                                  \
            for (idx_t t = 0; t <= max_t; ++t) {                    \
                idx_t j = i - t * (k);                              \
                sum += dp[j] * (seq_t)(t + 1);                      \
            }                                                       \
            next[i] = sum;                                          \
        }

    /* Apply the squared factors for k = 2, 3, 4, 5 */
    CONVOLVE_K((idx_t)2);
    seq_t *tmp = dp; dp = next; next = tmp;

    CONVOLVE_K((idx_t)3);
    tmp = dp; dp = next; next = tmp;

    CONVOLVE_K((idx_t)4);
    tmp = dp; dp = next; next = tmp;

    CONVOLVE_K((idx_t)5);
    tmp = dp; dp = next; next = tmp;

    #undef CONVOLVE_K

    /* Multiply by (1-x)^-1: simple prefix sums */
    for (idx_t i = 1; i <= n; ++i) {
        dp[i] += dp[i - 1];
    }

    /* Multiply by (1-x^6)^-1: add the term 6 positions earlier */
    for (idx_t i = 6; i <= n; ++i) {
        dp[i] += dp[i - 6];
    }

    seq_t result = dp[n];

    /* Clean up */
    free(dp);
    free(next);

    return result;
}
