/*
 * A000866  2^n written in base 5.
 * https://oeis.org/A000866
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = 2^n written in base 5
 * Range:     u8 n <= 6, u16 n <= 11, u32 n <= 23, u64 n <= 45, u128 n <= 90,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    // Compute 2^n using binary exponentiation
    seq_t power = 1;
    seq_t base = (seq_t)2;
    seq_t exp = (seq_t)n;
    
    while (exp > 0) {
        if (exp & 1) power = power * base;
        base = base * base;
        exp = exp >> 1;
    }
    
    // Convert to base 5 and assemble as decimal
    seq_t result = 0;
    seq_t multiplier = 1;
    
    while (power > 0) {
        result = result + (power % 5) * multiplier;
        multiplier = multiplier * 10;
        power = power / 5;
    }
    
    return result;
}
