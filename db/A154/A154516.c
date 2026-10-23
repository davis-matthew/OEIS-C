/*
 * A154516  a(n) = 9n^2 - n.
 * https://oeis.org/A154516
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 9*n*n - n
 * Range:     u8 n <= 5, u16 n <= 85, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t nn = (seq_t)n;
    return nn * ((seq_t)9 * nn - 1);
}
