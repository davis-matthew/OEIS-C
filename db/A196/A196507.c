/*
 * A196507  a(n) = n*(3*n^2 + 6*n + 1).
 * https://oeis.org/A196507
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(3*n^2 + 6*n + 1)
 * Range:     u8 n <= 3, u16 n <= 27, u32 n <= 1126, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t tn = (seq_t)n;
    seq_t n2 = tn * tn;
    seq_t res = tn * ((seq_t)3 * n2 + (seq_t)6 * tn + (seq_t)1);
    return res;
}
