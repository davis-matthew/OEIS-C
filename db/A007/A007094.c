/*
 * A007094  Numbers in base 8.
 * https://oeis.org/A007094
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, base
 * Formula:   a(n) is the decimal interpretation of n's base-8 representation
 * Range:     u8 n <= 173, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t result = 0;
    seq_t power_of_10 = 1;
    idx_t temp = n;
    
    while (temp > 0) {
        seq_t digit = (seq_t)(temp % 8);
        result += digit * power_of_10;
        power_of_10 *= 10;
        temp /= 8;
    }
    
    return result;
}
