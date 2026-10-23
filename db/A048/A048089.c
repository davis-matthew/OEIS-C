/*
 * A048089  Number of nonempty subsets of {1,2,...,n} in which exactly 3/5 of
 *          the elements are <= (n+3)/3.
 * https://oeis.org/A048089
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sum_{k≥1, 5k≤n} binomial(floor((n+3)/3), 3k) *
 *            binomial(n-fLo, 2k) where fLo = floor((n+3)/3)
 * Range:     u8 n <= 12, u16 n <= 23, u32 n <= 36, u64 n <= 36, u128 n <= 36,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static seq_t binom_calc(idx_t n, idx_t k) {
    if (k > n) return 0;
    if (k == 0 || k == n) return 1;
    if (k > n - k) k = n - k;
    seq_t res = 1;
    for (idx_t i = 1; i <= k; ++i) {
        res *= (seq_t)(n - k + i);
        res /= (seq_t)i;
    }
    return res;
}

seq_t a(idx_t n) {
    if (n < 6) return 0;
    idx_t L = (n + 3) / 3;        /* floor((n+3)/3)  */
    idx_t H = n - L;              /* elements > (n+3)/3  */
    seq_t result = 0;
    for (idx_t k = 1; 5 * k <= n; ++k) {
        idx_t low  = 3 * k;
        idx_t high = 2 * k;
        if (low > L || high > H) continue;
        seq_t term = binom_calc(L, low) * binom_calc(H, high);
        result += term;
    }
    return result;
}
