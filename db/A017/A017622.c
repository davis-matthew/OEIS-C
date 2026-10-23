/*
 * A017622  a(n) = (12*n+8)^6.
 * https://oeis.org/A017622
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (12*n+8)^6
 * Range:     u8 none, u16 none, u32 n <= 2, u64 n <= 134, u128 n <= 1000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t m = (seq_t)12 * n + (seq_t)8;
    seq_t r = 1;
    for (int i = 0; i < 6; ++i) {
        r *= m;
    }
    return r;
}
