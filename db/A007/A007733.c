/*
 * A007733  Period of binary representation of 1/n. Also, multiplicative order
 *          of 2 modulo the odd part of n (= A000265(n)).
 * https://oeis.org/A007733
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = A002326((A000265(n) - 1)/2), where A000265(n) is the odd
 *            part of n, and A002326(m) is the multiplicative order of 2
 *            modulo (2m+1)
 * Range:     u8 n <= 268, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    // Find the odd part of n
    idx_t odd_part = n;
    while (odd_part % 2 == 0) {
        odd_part /= 2;
    }
    
    // Compute A002326((odd_part - 1) / 2)
    idx_t m = (odd_part - 1) / 2;
    idx_t modulus = 2 * m + 1;
    
    if (modulus == 1) {
        return (seq_t)1;
    }
    
    // Find multiplicative order of 2 modulo modulus
    seq_t power = (seq_t)2 % (seq_t)modulus;
    seq_t order = (seq_t)1;
    
    while (power != 1) {
        power = (power * (seq_t)2) % (seq_t)modulus;
        order++;
    }
    
    return order;
}
