/*
 * A003564  Least number m such that 8^m == +- 1 (mod 8n + 1).
 * https://oeis.org/A003564
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   Least m such that 8^m ≡ ±1 (mod 8n+1)
 * Range:     u8 n <= 31, u16 n <= 70, u32 n <= 70, u64 n <= 70, u128 n <= 70,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t modulus = (seq_t)8 * n + 1;
    seq_t power = 8 % modulus;
    seq_t m = 1;
    
    while (1) {
        if (power == 1 || power == modulus - 1) {
            return m;
        }
        power = (power * 8) % modulus;
        m++;
    }
}
