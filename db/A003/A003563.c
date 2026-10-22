/*
 * A003563  Least number m such that 7^m == +- 1 (mod 7n + 1).
 * https://oeis.org/A003563
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   Least m such that 7^m == ±1 (mod 7n+1)
 * Range:     u8 n <= 36, u16 n <= 68, u32 n <= 68, u64 n <= 68, u128 n <= 68,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t modulus = (seq_t)7 * n + 1;
    seq_t power = 7 % modulus;
    seq_t m = 1;
    
    while (1) {
        if (power == 1 || power == modulus - 1) {
            return m;
        }
        power = (power * (seq_t)7) % modulus;
        m++;
    }
}
