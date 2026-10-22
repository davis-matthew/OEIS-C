/*
 * A003438  Number of 5 X 5 matrices with nonnegative integer entries and row
 *          and column sums equal to n.
 * https://oeis.org/A003438
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   a(n) = Sum_{j=0..12} c[j] * binomial(n - j + 16, 16), where c
 *            are the coefficients from the GF numerator
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 7, u64 n <= 39, u128 n <= 663,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    static const seq_t c[13] = {
        1, 103, 4306, 63110, 388615, 1115068, 1575669,
        1115068, 388615, 63110, 4306, 103, 1
    };
    
    seq_t result = 0;
    seq_t n_seq = (seq_t)n;
    
    for (int j = 0; j <= 12; j++) {
        if ((idx_t)j > n) break;
        
        seq_t m = n_seq + 16 - j;
        seq_t binom_val = 1;
        
        /* Compute binomial(m, 16) iteratively */
        for (int i = 1; i <= 16; i++) {
            binom_val = binom_val * (m - 16 + i) / i;
        }
        
        result = result + c[j] * binom_val;
    }
    
    return result;
}
