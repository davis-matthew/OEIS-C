/*
 * A201807  Number of arrays of n integers in -4..4 with sum zero and equal
 *          numbers of elements greater than zero and less than zero.
 * https://oeis.org/A201807
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 12, u64 n <= 22, u128 n <= 42,
 *            big any n
 * Helpers:   seq_mul
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = Σ_{p=0}^{⌊n/2⌋} C(n,2p) · C(2p,p) · Σ_{s} f[p,s]²
 * where f[p,s] is the number of length‑p sequences of 1..4 with sum s.
 *
 * The code uses dynamic programming for f[p,s] and iterative
 * binomial updates.  It works for any integer type seq_t and idx_t
 * supplied by the build system (including arbitrary‑precision integers).
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/seq_mul.h"

seq_t a(idx_t n)
{
    /* maximum number of positive (and negative) elements */
    idx_t p_max = n / 2;

    /* maximum possible sum of positive (or negative) elements */
    size_t maxSum = 4 * p_max;

    /* DP arrays: dp[s] holds f[p,s] for current p */
    seq_t *dp = (seq_t *)calloc(maxSum + 1, sizeof(seq_t));
    seq_t *next = (seq_t *)calloc(maxSum + 1, sizeof(seq_t));
    if (!dp || !next) {
        free(dp);
        free(next);
        return 0;
    }

    dp[0] = 1;         /* f[0,0] = 1 */

    seq_t binom_n_2p = 1;   /* C(n, 2p)   for current p */
    seq_t binom_2p_p = 1;   /* C(2p, p)    for current p */
    seq_t result = 0;

    for (idx_t p = 0; p <= p_max; ++p) {
        /* sum of squares of f[p,s] */
        seq_t sumsq = 0;
        idx_t lower = p;
        idx_t upper = 4 * p;
        if (upper > maxSum) upper = maxSum;
        for (idx_t s = lower; s <= upper; ++s) {
            if (dp[s]) {
                seq_t tmp = seq_mul(dp[s], dp[s]);
                sumsq += tmp;
            }
        }

        /* add contribution for this p */
        seq_t term = seq_mul(seq_mul(binom_n_2p, binom_2p_p), sumsq);
        result += term;

        if (p == p_max)
            break;   /* no more iterations */

        /* update binomial coefficients for next p (+1) */
        /* C(n, 2(p+1)) = C(n, 2p) * (n-2p)(n-2p-1) / ((2p+1)(2p+2)) */
        seq_t num1 = seq_mul(binom_n_2p,
                             seq_mul((seq_t)(n - 2 * p),
                                     (seq_t)(n - 2 * p - 1)));
        seq_t den1 = (seq_t)(2 * p + 1) * (seq_t)(2 * p + 2);
        binom_n_2p = num1 / den1;

        /* C(2(p+1), p+1) = C(2p, p) * (2p+2)(2p+1) / ((p+1)²) */
        seq_t num2 = seq_mul(binom_2p_p,
                             seq_mul((seq_t)(2 * p + 2),
                                     (seq_t)(2 * p + 1)));
        seq_t den2 = (seq_t)(p + 1) * (seq_t)(p + 1);
        binom_2p_p = num2 / den2;

        /* compute f[p+1, *] from f[p, *] */
        size_t len_next = 4 * (p + 1) + 1;
        memset(next, 0, len_next * sizeof(seq_t));

        for (idx_t s = lower; s <= upper; ++s) {
            if (!dp[s]) continue;
            for (int k = 1; k <= 4; ++k) {
                next[s + k] += dp[s];
            }
        }

        /* swap dp and next */
        seq_t *tmp_arr = dp;
        dp = next;
        next = tmp_arr;
    }

    free(dp);
    free(next);
    return result;
}
