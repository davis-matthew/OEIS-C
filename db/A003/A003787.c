/*
 * A003787  Order of universal Chevalley group A_n (3).
 * https://oeis.org/A003787
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3^(n*(n+1)/2) * Product_{k=2..n+1}(3^k-1)
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 3, u64 n <= 5, u128 n <= 8,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t result = 1;
    
    // Compute 3^(n*(n+1)/2) using fast exponentiation
    seq_t exponent = ((seq_t)n * ((seq_t)n + 1)) / 2;
    seq_t power_of_3 = 1;
    seq_t base = 3;
    
    seq_t exp = exponent;
    while (exp > 0) {
        if (exp & 1) {
            power_of_3 *= base;
        }
        base *= base;
        exp >>= 1;
    }
    
    result = power_of_3;
    
    // Compute Product_{k=2..n+1}(3^k-1)
    seq_t three_to_k = 9;  // 3^2
    for (idx_t k = 2; k <= n + 1; k++) {
        result *= (three_to_k - 1);
        three_to_k *= 3;  // Update to 3^(k+1)
    }
    
    return result;
}
