/*
 * A001551  a(n) = 1^n + 2^n + 3^n + 4^n.
 * https://oeis.org/A001551
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 1^n + 2^n + 3^n + 4^n
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 15, u64 n <= 31, u128 n <= 63,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t p2 = (seq_t)1, p3 = (seq_t)1, p4 = (seq_t)1;
    idx_t i;
    for (i = 0; i < n; i++) {
        p2 *= 2;
        p3 *= 3;
        p4 *= 4;
    }
    return (seq_t)1 + p2 + p3 + p4;
}
