/*
 * A004660  Powers of 3 written in base 6.
 * https://oeis.org/A004660
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = 3^n represented in base 6, with digits read as base 10
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 16, u64 n <= 31, u128 n <= 63,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static seq_t pow3(idx_t n) {
    seq_t result = 1;
    seq_t base = 3;
    
    while (n > 0) {
        if (n & 1) {
            result *= base;
        }
        base *= base;
        n >>= 1;
    }
    
    return result;
}

seq_t a(idx_t n) {
    seq_t power_of_3 = pow3(n);
    
    seq_t result = 0;
    seq_t multiplier = 1;
    
    while (power_of_3 > 0) {
        seq_t digit = power_of_3 % 6;
        result += digit * multiplier;
        multiplier *= 10;
        power_of_3 /= 6;
    }
    
    return result;
}
