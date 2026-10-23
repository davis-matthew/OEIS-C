/*
 * A152741  13 times triangular numbers.
 * https://oeis.org/A152741
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 13*n*(n+1)/2
 * Range:     u8 n <= 5, u16 n <= 99, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t nn = (seq_t)n;
    seq_t t  = nn * (nn + 1) / 2;   /* triangular number */
    return t * 13;
}
