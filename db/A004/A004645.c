/*
 * A004645  Powers of 2 written in base 6.
 * https://oeis.org/A004645
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = 2^n written in base 6, with base-6 digits read as decimal
 * Range:     u8 n <= 6, u16 n <= 12, u32 n <= 25, u64 n <= 50, u128 n <= 100,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    // Compute 2^n
    seq_t power_of_2 = 1;
    for (idx_t i = 0; i < n; i++) {
        power_of_2 *= 2;
    }
    
    // Convert 2^n from base 6 to decimal representation
    seq_t result = 0;
    seq_t multiplier = 1;
    
    while (power_of_2 > 0) {
        seq_t digit = power_of_2 % 6;
        result += digit * multiplier;
        multiplier *= 10;
        power_of_2 /= 6;
    }
    
    return result;
}
