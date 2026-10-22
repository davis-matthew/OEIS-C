/*
 * A004217  a(n) = (n^n)^(n^n).
 * https://oeis.org/A004217
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n^n)^(n^n) = n^(n * n^n)
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 2, u64 n <= 2, u128 n <= 2,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t base = (seq_t)n;
    
    if (n == 0) {
        return 1;
    }
    
    if (n == 1) {
        return 1;
    }
    
    // Compute n^n using binary exponentiation
    seq_t n_to_n = 1;
    seq_t temp_base = base;
    seq_t temp_exp = base;
    while (temp_exp > 0) {
        if (temp_exp & 1) {
            n_to_n *= temp_base;
        }
        temp_base *= temp_base;
        temp_exp >>= 1;
    }
    
    // Compute exponent = n * n^n
    seq_t exponent = base * n_to_n;
    
    // Compute n^exponent using binary exponentiation
    seq_t result = 1;
    base = (seq_t)n;
    while (exponent > 0) {
        if (exponent & 1) {
            result *= base;
        }
        base *= base;
        exponent >>= 1;
    }
    
    return result;
}
