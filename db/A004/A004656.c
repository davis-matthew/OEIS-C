/*
 * A004656  Powers of 3 written in base 2.
 * https://oeis.org/A004656
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = 3^n written in base 2, interpreted as decimal
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 12, u128 n <= 24,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
        
    // Compute 3^n using fast exponentiation
    seq_t power = 1;
    seq_t base = 3;
    idx_t exp = n;
    while (exp > 0) {
        if (exp & 1) {
            power *= base;
        }
        base *= base;
        exp >>= 1;
    }
    
    // Extract bits and build decimal representation
    seq_t result = 0;
    seq_t bit_mask = 1;
    while (bit_mask <= power) {
        bit_mask <<= 1;
    }
    bit_mask >>= 1;
    
    while (bit_mask > 0) {
        result = result * 10 + ((power & bit_mask) ? 1 : 0);
        bit_mask >>= 1;
    }
    
    return result;
}
