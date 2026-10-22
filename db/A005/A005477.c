/*
 * A005477  a(n) = 2^(n-1)*(2^n - 1)*Product_{j=1..n-1} (2^j + 1).
 * https://oeis.org/A005477
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2^(n-1)*(2^n - 1)*Product_{j=1..n-1} (2^j + 1)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 6, u64 n <= 9, u128 n <= 14,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 0) return 0;
    
    seq_t pow2_n_minus_1 = 1;
    for (idx_t i = 0; i < n - 1; i++) {
        pow2_n_minus_1 *= 2;
    }
    
    seq_t pow2_n_minus_1_val = pow2_n_minus_1 * 2 - 1;
    
    seq_t result = pow2_n_minus_1 * pow2_n_minus_1_val;
    
    seq_t pow2_j = 2;
    for (idx_t j = 1; j < n; j++) {
        result *= (pow2_j + 1);
        pow2_j *= 2;
    }
    
    return result;
}
