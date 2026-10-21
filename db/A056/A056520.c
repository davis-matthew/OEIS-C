/*
 * A056520  a(n) = (n + 2)*(2*n^2 - n + 3)/6.
 * https://oeis.org/A056520
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n + 2)*(2*n^2 - n + 3)/6
 * Range:     u8 n <= 4, u16 n <= 31, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;
    seq_t num = (nn + 2) * ( ((seq_t)2 * nn * nn) - nn + (seq_t)3 );
    return num / (seq_t)6;
}
