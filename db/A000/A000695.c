/*
 * A000695  Moser-de Bruijn sequence: sums of distinct powers of 4.
 * https://oeis.org/A000695
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, nice, easy
 * Formula:   To get a(n), write n as Sum b_j*2^j, then a(n) = Sum b_j*2^(2j)
 * Range:     u8 n <= 15, u16 n <= 255, u32 n <= 1023, u64 n <= 1023,
 *            u128 n <= 1023, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t result = 0;
    seq_t power_of_4 = 1;
    
    while (n > 0) {
        if (n & 1) {
            result += power_of_4;
        }
        n >>= 1;
        power_of_4 *= 4;
    }
    
    return result;
}
