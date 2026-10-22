/*
 * A005061  a(n) = 4^n - 3^n.
 * https://oeis.org/A005061
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 4^n - 3^n
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 16, u64 n <= 32, u128 n <= 64,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t result4 = 1;
    seq_t result3 = 1;
    seq_t base4 = 4;
    seq_t base3 = 3;
    idx_t exp = n;
    
    while (exp > 0) {
        if (exp & 1) {
            result4 *= base4;
            result3 *= base3;
        }
        base4 *= base4;
        base3 *= base3;
        exp >>= 1;
    }
    
    return result4 - result3;
}
