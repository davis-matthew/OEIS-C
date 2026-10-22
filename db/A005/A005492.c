/*
 * A005492  From expansion of falling factorials.
 * https://oeis.org/A005492
 *
 * Offset:    4    (a(4) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n^4 - 16*n^3 + 102*n^2 - 300*n + 340
 * Range:     u8 n <= 7, u16 n <= 19, u32 n <= 259, u64 n <= 1004,
 *            u128 n <= 1004, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t nn = (seq_t)n;
    seq_t n2 = nn * nn;
    seq_t n3 = n2 * nn;
    seq_t n4 = n3 * nn;
    
    return n4 - 16*n3 + 102*n2 - 300*nn + 340;
}
