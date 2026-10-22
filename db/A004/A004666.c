/*
 * A004666  Powers of 3 written in base 12. (Next term contains a non-decimal
 *          character.)
 * https://oeis.org/A004666
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = base-12 representation of 3^n, interpreted as base-10
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 8, u64 n <= 8, u128 n <= 8,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    // Compute 3^n using binary exponentiation
    seq_t power = 1;
    seq_t base = 3;
    idx_t exp = n;
    
    while (exp > 0) {
        if (exp & 1) {
            power = power * base;
        }
        base = base * base;
        exp = exp >> 1;
    }
    
    // Extract base-12 digits and interpret as base-10
    seq_t result = 0;
    seq_t multiplier = 1;
    
    while (power > 0) {
        seq_t digit = power % 12;
        result = result + digit * multiplier;
        multiplier = multiplier * 10;
        power = power / 12;
    }
    
    return result;
}
