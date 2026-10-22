/*
 * A004140  Number of nonempty labeled simple graphs on nodes chosen from an
 *          n-set.
 * https://oeis.org/A004140
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, nice
 * Formula:   a(n) = Sum_{k=1..n} binomial(n, k)*2^(k(k-1)/2)
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 8, u64 n <= 11, u128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t result = 0;
    seq_t binom_nk = (seq_t)n;  // C(n, 1) = n
    
    for (idx_t k = 1; k <= n; k++) {
        idx_t exp = k * (k - 1) / 2;
        seq_t pow2 = (seq_t)1 << exp;
        result += binom_nk * pow2;
        
        if (k < n) {
            binom_nk = binom_nk * ((seq_t)n - (seq_t)k) / ((seq_t)k + 1);
        }
    }
    
    return result;
}
