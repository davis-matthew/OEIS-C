/*
 * A001593  a(n) = 5^n + n^5.
 * https://oeis.org/A001593
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 5^n + n^5
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 13, u64 n <= 27, u128 n <= 55,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t n_val = (seq_t)n;
    
    // Compute n^5 by repeated multiplication
    seq_t n2 = n_val * n_val;
    seq_t n4 = n2 * n2;
    seq_t n5 = n4 * n_val;
    
    // Compute 5^n by binary exponentiation
    seq_t pow_5n = 1;
    seq_t base = 5;
    idx_t exp = n;
    
    while (exp > 0) {
        if (exp & 1) {
            pow_5n *= base;
        }
        base *= base;
        exp >>= 1;
    }
    
    return pow_5n + n5;
}
