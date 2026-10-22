/*
 * A007736  Period of repeating digits of 1/n in base 5.
 * https://oeis.org/A007736
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = multiplicative order of 5 modulo (n / 5^v_5(n))
 * Range:     u8 n <= 215, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t n_val = (seq_t)n;
    
    // Compute n' = n / 5^v_5(n), where v_5(n) is the 5-adic valuation
    seq_t n_prime = n_val;
    while (n_prime % 5 == 0) {
        n_prime /= 5;
    }
    
    // Handle n' = 1
    if (n_prime == 1) return 1;
    
    // Find the multiplicative order of 5 modulo n'
    seq_t remainder = 5 % n_prime;
    seq_t order = 1;
    
    while (remainder != 1) {
        remainder = (remainder * 5) % n_prime;
        order++;
    }
    
    return order;
}
