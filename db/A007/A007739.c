/*
 * A007739  Period of repeating digits of 1/n in base 8.
 * https://oeis.org/A007739
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = multiplicative order of 8 modulo (odd part of n)
 * Range:     u8 n <= 267, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    // Remove all factors of 2 from n to get the odd part
    seq_t n_reduced = (seq_t)n;
    while (n_reduced % 2 == 0) {
        n_reduced /= 2;
    }
    
    // If the odd part is 1, the decimal terminates
    if (n_reduced == 1) {
        return 1;
    }
    
    // Find the multiplicative order of 8 modulo n_reduced
    seq_t power = 8 % n_reduced;
    seq_t k = 1;
    
    while (power != 1) {
        power = (power * 8) % n_reduced;
        k++;
    }
    
    return k;
}
