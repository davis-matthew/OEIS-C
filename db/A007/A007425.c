/*
 * A007425  d_3(n), or tau_3(n), the number of ordered factorizations of n as
 *          n = r s t.
 * https://oeis.org/A007425
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, nice, easy, mult, changed
 * Formula:   a(n) = Product_i T(e_i + 1) where n = Product_i p_i^e_i and T(k)
 *            = k*(k+1)/2
 * Range:     u8 n <= 719, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 1) return 1;
    
    seq_t result = 1;
    idx_t temp = n;
    
    // Trial division to find prime factors and their exponents
    for (idx_t p = 2; p * p <= temp; p++) {
        if (temp % p == 0) {
            idx_t exponent = 0;
            while (temp % p == 0) {
                exponent++;
                temp /= p;
            }
            // Compute triangular number T(exponent + 1)
            idx_t k = exponent + 1;
            seq_t tri = ((seq_t)k * (seq_t)(k + 1)) / 2;
            result *= tri;
        }
    }
    
    // If temp > 1, then it's a prime factor with exponent 1, contributing T(2) = 3
    if (temp > 1) {
        result *= 3;
    }
    
    return result;
}
