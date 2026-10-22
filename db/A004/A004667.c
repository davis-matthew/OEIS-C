/*
 * A004667  Powers of 3 written in base 13. (Next term contains a non-decimal
 *          digit.)
 * https://oeis.org/A004667
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = interpret as base-10 the digits of 3^n when written in
 *            base 13
 * Range:     u8 n <= 5, u16 n <= 6, u32 n <= 6, u64 n <= 6, u128 n <= 6,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    // Compute 3^n using fast exponentiation
    seq_t power = (seq_t)1;
    seq_t base = (seq_t)3;
    idx_t exp = n;
    
    while (exp > 0) {
        if (exp & 1) {
            power = power * base;
        }
        base = base * base;
        exp >>= 1;
    }
    
    // Extract base-13 digits and reconstruct as base-10
    seq_t result = (seq_t)0;
    seq_t multiplier = (seq_t)1;
    
    while (power > 0) {
        seq_t digit = power % (seq_t)13;
        result = result + digit * multiplier;
        multiplier = multiplier * (seq_t)10;
        power = power / (seq_t)13;
    }
    
    return result;
}
