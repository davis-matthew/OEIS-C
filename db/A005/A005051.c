/*
 * A005051  a(n) = 8*3^n.
 * https://oeis.org/A005051
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 8*3^n
 * Range:     u8 n <= 3, u16 n <= 8, u32 n <= 18, u64 n <= 38, u128 n <= 78,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t result = 1;
    seq_t base = 3;
    
    while (n > 0) {
        if (n & 1) {
            result *= base;
        }
        base *= base;
        n >>= 1;
    }
    
    return (seq_t)8 * result;
}
