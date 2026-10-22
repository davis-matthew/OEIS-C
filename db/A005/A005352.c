/*
 * A005352  Base -2 representation of -n reinterpreted as binary.
 * https://oeis.org/A005352
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, nice, look
 * Formula:   a(n) = negabinary(-n) interpreted as binary
 * Range:     u8 n <= 170, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    long long m = -(long long)n;
    seq_t result = 0;
    seq_t bit_pos = 1;
    
    while (m != 0) {
        if (m % 2 == 0) {
            m = m / (-2);
        } else {
            result = result + bit_pos;
            m = (m - 1) / (-2);
        }
        bit_pos = bit_pos * 2;
    }
    
    return result;
}
