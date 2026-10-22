/*
 * A003789  Order of universal Chevalley group A_n (5).
 * https://oeis.org/A003789
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 5^(n(n+1)/2) * ∏(k=2 to n+1)(5^k - 1)
 * Range:     u8 n <= 1, u16 n <= 1, u32 n <= 2, u64 n <= 4, u128 n <= 6,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t result = 1;
    
    // Compute 5^(n(n+1)/2) using fast exponentiation
    idx_t exp = n * (n + 1) / 2;
    seq_t base = 5;
    while (exp > 0) {
        if (exp & 1) {
            result *= base;
        }
        base *= base;
        exp >>= 1;
    }
    
    // Compute the product ∏(k=2 to n+1)(5^k - 1)
    seq_t power_of_5_k = 25;  // 5^2
    for (idx_t k = 2; k <= n + 1; k++) {
        result *= (power_of_5_k - 1);
        power_of_5_k *= 5;
    }
    
    return result;
}
