/*
 * A135705  a(n) = 10*binomial(n,2) + 9*n.
 * https://oeis.org/A135705
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(5*n+4)
 * Range:     u8 n <= 6, u16 n <= 114, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t nn = (seq_t)n;
    return nn * ((seq_t)5 * nn + (seq_t)4);
}
