/*
 * A074759  Number of degree-n permutations of order dividing n. Number of
 *          solutions to x^n = 1 in S_n.
 * https://oeis.org/A074759
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 1 + Σ_{k|n,k>0} (n−1)!/(n−k)! * a(n−k), where the sum
 *            runs over all positive divisors k of n. This counts permutations
 *            of n elements with all cycle lengths dividing n
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 13, u64 n <= 21, u128 n <= 35,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Handle trivial case */
    if (n == 0) return (seq_t)1;

    /* Compute all divisors of n (they are few for n ≤ 10000) */
    idx_t divs[128];
    size_t dcnt = 0;
    for (idx_t d = 1; d * d <= n; ++d) {
        if (n % d == 0) {
            divs[dcnt++] = d;
            if (d != n / d) divs[dcnt++] = n / d;
        }
    }

    /* Allocate temporary arrays for factorials and DP values */
    seq_t *fact = (seq_t *)calloc((size_t)n + 1, sizeof(seq_t));
    seq_t *dp   = (seq_t *)calloc((size_t)n + 1, sizeof(seq_t));
    if (!fact || !dp) {  /* allocation failure – return 0 as graceful fail */
        free(fact);
        free(dp);
        return (seq_t)0;
    }

    /* Pre‑compute factorials */
    fact[0] = (seq_t)1;
    for (idx_t i = 1; i <= n; ++i) {
        fact[i] = fact[i - 1] * (seq_t)i;
    }

    /* DP base */
    dp[0] = (seq_t)1;

    /* Main DP loop: compute dp[m] for 1 ≤ m ≤ n */
    for (idx_t m = 1; m <= n; ++m) {
        seq_t sum = (seq_t)0;
        for (size_t i = 0; i < dcnt; ++i) {
            idx_t k = divs[i];
            if (k > m) continue;   /* divisor larger than current size can't be used */

            /* (m−1)! / (m−k)!  =  fact[m-1] / fact[m-k]  (exact division) */
            seq_t ratio = fact[m - 1] / fact[m - k];
            sum += ratio * dp[m - k];
        }
        dp[m] = sum;
    }

    seq_t result = dp[n];
    free(fact);
    free(dp);
    return result;
}
