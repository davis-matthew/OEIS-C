/*
 * A000420  Powers of 7: a(n) = 7^n.
 * https://oeis.org/A000420
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 7^n
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 11, u64 n <= 22, u128 n <= 45,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t result = 1;
    seq_t base = 7;
    while (n > 0) {
        if (n & 1) {
            result *= base;
        }
        base *= base;
        n >>= 1;
    }
    return result;
}
