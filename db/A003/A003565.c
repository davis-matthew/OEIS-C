/*
 * A003565  Least number m such that 9^m == +- 1 (mod 9n + 1).
 * https://oeis.org/A003565
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = least m such that 9^m ≡ ±1 (mod 9n+1)
 * Range:     u8 n <= 28, u16 n <= 67, u32 n <= 67, u64 n <= 67, u128 n <= 67,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t modulus = (seq_t)n * 9 + 1;
    seq_t power = 9 % modulus;
    
    for (seq_t m = 1; m < modulus; m++) {
        if (power == 1 || power == modulus - 1) {
            return m;
        }
        power = (power * 9) % modulus;
    }
    
    return modulus;
}
