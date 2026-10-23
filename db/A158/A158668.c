/*
 * A158668  a(n) = 58*n^2 - 1.
 * https://oeis.org/A158668
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 58*n^2 - 1
 * Range:     u8 n <= 2, u16 n <= 33, u32 n <= 8605, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)0;
    seq_t nn = (seq_t)n;
    seq_t res = nn * nn;
    res = res * (seq_t)58;
    res = res - (seq_t)1;
    return res;
}
