/*
 * A001025  Powers of 16: a(n) = 16^n.
 * https://oeis.org/A001025
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 16^n
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 15, u128 n <= 31,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t result = 1;
    seq_t base = 16;
    
    while (n > 0) {
        if (n & 1) {
            result *= base;
        }
        base *= base;
        n >>= 1;
    }
    return result;
}
