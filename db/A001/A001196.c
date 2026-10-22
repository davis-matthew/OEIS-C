/*
 * A001196  Double-bitters: only even length runs in binary expansion.
 * https://oeis.org/A001196
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(2n) = 4*a(n), a(2n+1) = 4*a(n) + 3, a(0) = 0
 * Range:     u8 n <= 15, u16 n <= 255, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t result = 0;
    seq_t power_of_4 = 1;
    
    while (n > 0) {
        if (n & 1) {
            result = result + 3 * power_of_4;
        }
        n = n >> 1;
        power_of_4 = power_of_4 * 4;
    }
    
    return result;
}
