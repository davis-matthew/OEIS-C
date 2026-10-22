/*
 * A003590  Rows of Pascal's triangle written as a single number.
 * https://oeis.org/A003590
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, base
 * Formula:   a(n) = concatenation of binomial coefficients C(n,k) for k=0..n
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 6, u64 n <= 9, u128 n <= 13,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static seq_t count_digits(seq_t x) {
    if (x == 0) return 1;
    seq_t count = 0;
    while (x > 0) {
        count++;
        x /= 10;
    }
    return count;
}

seq_t a(idx_t n) {
    seq_t result = 0;
    seq_t binom = 1;
    
    for (idx_t k = 0; k <= n; k++) {
        if (k > 0) {
            binom = binom * (seq_t)(n - k + 1) / (seq_t)k;
        }
        
        seq_t digits = count_digits(binom);
        seq_t shift = 1;
        for (seq_t i = 0; i < digits; i++) {
            shift = shift * 10;
        }
        result = result * shift + binom;
    }
    
    return result;
}
