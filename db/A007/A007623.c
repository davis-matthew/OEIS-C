/*
 * A007623  Integers written in factorial base.
 * https://oeis.org/A007623
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, nonn, nice, easy
 * Formula:   Convert n to factorial base (d_i where n = sum_{i>=1} d_i*i!, 0
 *            <= d_i <= i), then concatenate digits d_1, d_2, ... to form the
 *            result
 * Range:     u8 n <= 17, u16 n <= 719, u32 n <= 9920, u64 n <= 9920,
 *            u128 n <= 9920, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 0) return 0;
    
    seq_t result = 0;
    idx_t quotient = n;
    idx_t i = 2;
    
    idx_t digits[64];
    idx_t num_digits = 0;
    
    while (quotient > 0) {
        digits[num_digits] = quotient % i;
        quotient = quotient / i;
        num_digits++;
        i++;
    }
    
    for (idx_t j = num_digits; j > 0; j--) {
        result = result * 10 + (seq_t)digits[j - 1];
    }
    
    return result;
}
