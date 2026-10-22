/*
 * A000468  Powers of ten written in base 8.
 * https://oeis.org/A000468
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy, nice
 * Formula:   a(n) = 10^n written in base 8, then read as a decimal number
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 17, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    // Compute 10^n
    seq_t power_of_10 = 1;
    for (idx_t i = 0; i < n; i++) {
        power_of_10 *= 10;
    }
    
    // Extract base 8 digits and rebuild as decimal
    seq_t result = 0;
    seq_t multiplier = 1;
    
    while (power_of_10 > 0) {
        seq_t digit = power_of_10 % 8;
        result += digit * multiplier;
        multiplier *= 10;
        power_of_10 /= 8;
    }
    
    return result;
}
