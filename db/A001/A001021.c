/*
 * A001021  Powers of 12: a(n) = 12^n.
 * https://oeis.org/A001021
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 12^n
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 17, u128 n <= 35,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t result = 1;
    seq_t base = (seq_t)12;
    
    while (n > 0) {
        if (n & 1) {
            result = result * base;
        }
        base = base * base;
        n >>= 1;
    }
    
    return result;
}
