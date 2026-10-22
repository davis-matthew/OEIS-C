/*
 * A007092  Numbers in base 6.
 * https://oeis.org/A007092
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, base
 * Formula:   a(n) = decimal representation of n written in base 6
 * Range:     u8 n <= 107, u16 n <= 7775, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t result = 0;
    seq_t power = 1;
    
    while (n > 0) {
        seq_t digit = (seq_t)(n % 6);
        result += digit * power;
        power *= 10;
        n /= 6;
    }
    
    return result;
}
