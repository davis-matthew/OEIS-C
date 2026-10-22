/*
 * A007740  Period of repeating digits of 1/n in base 9.
 * https://oeis.org/A007740
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   multiplicative order of 9 modulo (n / gcd(n, 9^∞))
 * Range:     u8 n <= 520, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 1) return 1;
    
    // Remove factors of 3 from n
    uint64_t n_reduced = (uint64_t)n;
    while (n_reduced % 3 == 0) {
        n_reduced /= 3;
    }
    
    if (n_reduced == 1) return 1;
    
    // Find the multiplicative order of 9 modulo n_reduced
    uint64_t power = 9 % n_reduced;
    uint64_t k = 1;
    
    while (power != 1) {
        power = (power * 9) % n_reduced;
        k++;
    }
    
    return (seq_t)k;
}
