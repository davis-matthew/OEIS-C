/*
 * A129824  a(n) = Product_{k=0..n} (1 + binomial(n,k)).
 * https://oeis.org/A129824
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = Product_{k=0..n} (1 + binomial(n,k))
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 7, u64 n <= 10, u128 n <= 14,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t ans = 1;
    seq_t comb = 1; /* binomial(n, 0) */

    for (idx_t k = 0; k <= n; ++k) {
        if (k > 0) {
            /* Update comb to binomial(n, k) */
            comb = comb * (seq_t)(n - k + 1);
            comb = comb / (seq_t)k;
        }
        seq_t term = comb + 1;
        ans = ans * term;
    }
    return ans;
}
