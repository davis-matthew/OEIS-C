/*
 * A007734  Period of repeating digits of 1/n in base 3.
 * https://oeis.org/A007734
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = multiplicative order of 3 modulo (n with all factors of 3
 *            removed)
 * Range:     u8 n <= 256, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    uint64_t m = n;
    
    // Remove all factors of 3
    while (m % 3 == 0) {
        m /= 3;
    }
    
    if (m == 1) {
        return 1;
    }
    
    // Find multiplicative order of 3 modulo m
    seq_t order = 1;
    uint64_t power = 3 % m;
    
    while (power != 1) {
        power = (power * 3) % m;
        order++;
    }
    
    return order;
}
