/*
 * A001496  Number of 4 X 4 matrices with nonnegative integer entries and row
 *          and column sums equal to n.
 * https://oeis.org/A001496
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   a(n) = binomial(n + 3, 3) + 20*binomial(n + 4, 5) +
 *            152*binomial(n + 5, 7) + 352*binomial(n + 6, 9)
 * Range:     u8 n <= 1, u16 n <= 5, u32 n <= 23, u64 n <= 296, u128 n <= 1000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static seq_t binomial(idx_t n, idx_t k) {
    if (k > n) return 0;
    if (k == 0) return 1;
    
    seq_t result = 1;
    for (idx_t i = 1; i <= k; i++) {
        result = result * (n - i + 1) / i;
    }
    return result;
}

seq_t a(idx_t n) {
    seq_t term1 = binomial(n + 3, 3);
    seq_t term2 = binomial(n + 4, 5) * 20;
    seq_t term3 = binomial(n + 5, 7) * 152;
    seq_t term4 = binomial(n + 6, 9) * 352;
    
    return term1 + term2 + term3 + term4;
}
