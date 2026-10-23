/*
 * A158325  a(n) = 484*n^2 + 2*n.
 * https://oeis.org/A158325
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 484*n^2 + 2*n
 * Range:     u8 none, u16 n <= 11, u32 n <= 2978, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;
    seq_t term = 484 * (nn * nn) + 2 * nn;
    return term;
}
