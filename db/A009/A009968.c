/*
 * A009968  Powers of 24: a(n) = 24^n.
 * https://oeis.org/A009968
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 24^n
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 13, u128 n <= 27,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t result = 1;
    seq_t base = 24;
    while (n) {
        if (n & 1) result = result * base;
        n >>= 1;
        base = base * base;
    }
    return result;
}
