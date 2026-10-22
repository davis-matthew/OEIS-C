/*
 * A007735  Period of base 4 representation of 1/n.
 * https://oeis.org/A007735
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   Period of the base 4 representation of 1/n
 * Range:     u8 n <= 522, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    uint64_t m = n;
    
    // Remove all factors of 2 from n
    while ((m & 1) == 0) {
        m >>= 1;
    }
    
    // If m = 1, the period is 1
    if (m == 1) {
        return 1;
    }
    
    // Compute the multiplicative order of 4 modulo m
    uint64_t order = 1;
    uint64_t power = 4 % m;
    
    while (power != 1) {
        power = (power * 4) % m;
        order++;
    }
    
    return (seq_t)order;
}
