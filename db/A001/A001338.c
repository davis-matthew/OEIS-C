/*
 * A001338  -1 + Sum (k-1)! C(n,k), k = 1..n for n > 0, a(0) = 1.
 * https://oeis.org/A001338
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = -1 + Sum_{k=1}^{n} (k-1)! * C(n,k), a(0) = 1
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 13, u64 n <= 21, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 0) return 1;
    
    seq_t result = 0;
    seq_t factorial_km1 = 1;
    seq_t binom = (seq_t)n;
    
    for (idx_t k = 1; k <= n; k++) {
        result += factorial_km1 * binom;
        
        if (k < n) {
            factorial_km1 *= (seq_t)k;
            binom = (binom * (seq_t)(n - k)) / (seq_t)(k + 1);
        }
    }
    
    return result - 1;
}
