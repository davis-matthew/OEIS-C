/*
 * A003559  Least number m such that 3^m == +- 1 (mod 3n + 1).
 * https://oeis.org/A003559
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = least m such that 3^m ≡ ±1 (mod 3n+1)
 * Range:     u8 n <= 71, u16 n <= 71, u32 n <= 71, u64 n <= 71, u128 n <= 71,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t modulus = (seq_t)n * 3 + 1;
    seq_t power = 3 % modulus;
    seq_t m = 1;
    
    while (power != 1 && power != modulus - 1) {
        power = (power * 3) % modulus;
        m = m + 1;
    }
    
    return m;
}
