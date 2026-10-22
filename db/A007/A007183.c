/*
 * A007183  Maximal splittance of a planar graph with n nodes.
 * https://oeis.org/A007183
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, nice, easy
 * Formula:   a(n) = 3*n - 24 for n >= 22; explicit values for n < 22
 * Range:     u8 n <= 93, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    static const seq_t table[] = {
        0, 0, 0, 1, 2, 3, 4, 6, 8, 10, 12, 15, 16, 19, 22, 25, 27, 30, 32, 35, 37, 40
    };
    
    if (n < 22) {
        return table[n];
    } else {
        return (seq_t)3 * (seq_t)n - (seq_t)24;
    }
}
