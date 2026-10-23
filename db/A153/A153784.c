/*
 * A153784  4 times heptagonal numbers: a(n) = 2*n*(5*n-3).
 * https://oeis.org/A153784
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 2*n*(5*n-3) = 10*n^2 - 6*n
 * Range:     u8 n <= 5, u16 n <= 81, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;
    return nn * ((seq_t)10 * nn - (seq_t)6);
}
