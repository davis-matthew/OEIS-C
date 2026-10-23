/*
 * A104274  Number of partitions of n in which odd squares occur with 2 types
 *          c,c* and with multiplicity 1. The even squares and parts that are
 *          twice the squares they occur with multiplicity 1.
 * https://oeis.org/A104274
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = coefficient of x^n in ∏_{k>0}
 *            ((1+x^{(2k-1)^2})/(1-x^{(2k-1)^2}))
 * Range:     u8 n <= 66, u16 n <= 66, u32 n <= 66, u64 n <= 66, u128 n <= 66,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* DP: iterate over odd squares s = 2k-1, let t = s*s. For each t, update dp:
   dp2[i] = dp[i] + 2 * Σ_{m≥1, i≥m*t} dp[i-m*t]  (sum on multiples of t). */
/* Only odd squares are restricted; all other parts are unrestricted, so the product
   includes only the odd squares factors as above. */

seq_t a(idx_t n) {
    /* Allocate two temporary arrays of size n+1 */
    idx_t sz = n + 1;
    seq_t *dp  = (seq_t *)calloc(sz, sizeof(seq_t));
    seq_t *dp2 = (seq_t *)calloc(sz, sizeof(seq_t));
    if (!dp || !dp2) return 0;  /* allocation failure */

    dp[0] = 1;  /* base case: one way to partition 0 */

    /* Process each odd square s = 2k-1 */
    for (idx_t k = 1;; ++k) {
        idx_t s = 2*k - 1;
        idx_t t = s * s;          /* square value */
        if (t > n) break;         /* no further contribution */

        /* For each residue class modulo t, accumulate sums */
        for (idx_t r = 0; r < t; ++r) {
            seq_t sum = 0;
            for (idx_t i = r; i <= n; i += t) {
                /* new[i] = old[i] + 2 * sum_of_old_at_multiples_of_t < i */
                dp2[i] = dp[i] + (seq_t)2 * sum;
                sum += dp[i];
            }
        }

        /* Prepare for the next odd square: dp becomes dp2 */
        seq_t *tmp = dp;
        dp = dp2;
        dp2 = tmp;
    }

    seq_t result = dp[n];

    /* Free the dynamically allocated arrays */
    free(dp);
    free(dp2);

    return result;
}
