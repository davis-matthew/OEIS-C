/*
 * A004478  Nim product 13 * n.
 * https://oeis.org/A004478
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   Nim product 13 * n
 * Range:     u8 n <= 58, u16 n <= 58, u32 n <= 58, u64 n <= 58, u128 n <= 58,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    static const seq_t table[16] = {
        0, 13, 6, 11, 9, 4, 15, 2, 14, 3, 8, 5, 7, 10, 1, 12
    };
    
    if (n < 16) {
        return table[n];
    }
    
    idx_t m = n / 16;
    idx_t r = n % 16;
    
    seq_t a_m = a(m);
    seq_t a_m_times_16 = a_m << 4;
    seq_t a_r = table[r];
    
    return a_m_times_16 ^ a_r;
}
