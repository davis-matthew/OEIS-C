/*
 * A017288  a(n) = (10*n + 1)^8.
 * https://oeis.org/A017288
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (10*n + 1)^8
 * Range:     u8 n <= 0, u16 n <= 0, u32 n <= 1, u64 n <= 25, u128 n <= 6553,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t x = (seq_t)n * ((seq_t)10) + 1;
    seq_t res = x;
    for (unsigned i = 1; i < 8; ++i) {
        res *= x;
    }
    return res;
}
