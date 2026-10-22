/*
 * A000534  Numbers that are not the sum of 4 nonzero squares.
 * https://oeis.org/A000534
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   Consists of the numbers 0, 1, 3, 5, 9, 11, 17, 29, 41, 2*4^m,
 *            6*4^m and 14*4^m (m >= 0). From a(19) on, a(n) = 4*a(n-3)
 * Range:     u8 n <= 19, u16 n <= 31, u32 n <= 55, u64 n <= 100,
 *            u128 n <= 100, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    static seq_t table[] = {
        0, 1, 2, 3, 5, 6, 8, 9, 11, 14, 17, 24, 29, 32, 41, 56, 96, 128, 224
    };
    
    if (n <= 18) {
        return table[n - 1];
    }
    
    idx_t k = (n - 16) / 3;
    idx_t base_index = n - 3 * k;
    seq_t result = table[base_index - 1];
    
    return result << (2 * k);
}
