/*
 * A178695  Expansion of the polynomial
 *          (1+x^3)*(1+x^5)*(1+x^7)*(1+x^9)*(1+x^11)*(1+x^13)*(1+x^15)*(1+x^17).
 * https://oeis.org/A178695
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   a(n) = coefficient of x^n in
 *            (1+x^3)*(1+x^5)*(1+x^7)*(1+x^9)*(1+x^11)*(1+x^13)*(1+x^15)*(1+x^17)
 * Range:     u8 n <= 80, u16 n <= 80, u32 n <= 80, u64 n <= 80, u128 n <= 80,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* DP array for all possible sums up to 80 (sum of all exponents) */
    static seq_t dp[81];
    static int initialized = 0;

    if (!initialized) {
        const idx_t exps[8] = {3,5,7,9,11,13,15,17};
        /* start with only the empty subset */
        dp[0] = 1;
        /* for each exponent add its contribution to all larger sums */
        for (int k = 0; k < 8; ++k) {
            idx_t e = exps[k];
            for (idx_t s = 80; s >= e; --s) {
                dp[s] += dp[s - e];
            }
        }
        initialized = 1;
    }

    return (n <= 80) ? dp[n] : (seq_t)0;
}
