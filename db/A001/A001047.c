/*
 * A001047  a(n) = 3^n - 2^n.
 * https://oeis.org/A001047
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   a(n) = 3^n - 2^n
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 20, u64 n <= 40, u128 n <= 80,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t p3 = 1;
    seq_t p2 = 1;
    idx_t i;
    for (i = 0; i < n; i++) {
        p3 *= 3;
        p2 *= 2;
    }
    return p3 - p2;
}
