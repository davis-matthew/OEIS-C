/*
 * A004943  a(n) = round(n*phi^8), where phi is the golden ratio, A001622.
 * https://oeis.org/A004943
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = round(n * phi^8)
 * Range:     u8 n <= 5, u16 n <= 1395, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
        
    const idx_t numerator = 514229;
    const idx_t denominator = 10946;
    
    // Compute using high-precision rational approximation: phi^8 ≈ 514229 / 10946
    // (514229/10946 is a convergent of the continued fraction for (47 + 21*sqrt(5))/2)
    idx_t product = n * numerator;
    idx_t quotient = product / denominator;
    idx_t remainder = product % denominator;
    
    // Round to nearest integer
    if (remainder * 2 >= denominator) {
        quotient++;
    }
    
    return (seq_t)quotient;
}
