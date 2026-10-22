/*
 * A003562  Least number m such that 6^m == +- 1 (mod 6n + 1).
 * https://oeis.org/A003562
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = least m such that 6^m == +- 1 (mod 6n + 1)
 * Range:     u8 n <= 42, u16 n <= 65, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t n_seq = (seq_t)n;
    seq_t modulus = 6 * n_seq + 1;
    seq_t power = 6 % modulus;
    
    for (seq_t k = 1; k < modulus; k++) {
        if (power == 1 || power == modulus - 1) {
            return k;
        }
        power = (power * 6) % modulus;
    }
    
    return modulus - 1;
}
