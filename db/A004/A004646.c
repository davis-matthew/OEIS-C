/*
 * A004646  Powers of 2 written in base 7.
 * https://oeis.org/A004646
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = base-7 representation of 2^n, read as a decimal number
 * Range:     u8 n <= 7, u16 n <= 14, u32 n <= 27, u64 n <= 54, u128 n <= 108,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t power_of_2 = 1;
    for (idx_t i = 0; i < n; i++) {
        power_of_2 *= 2;
    }
    
    seq_t result = 0;
    seq_t multiplier = 1;
    
    while (power_of_2 > 0) {
        seq_t digit = power_of_2 % 7;
        result += digit * multiplier;
        multiplier *= 10;
        power_of_2 /= 7;
    }
    
    return result;
}
