/*
 * A180223  a(n) = (11*n^2 - 7*n)/2.
 * https://oeis.org/A180223
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (11*n^2 - 7*n)/2
 * Range:     u8 n <= 7, u16 n <= 109, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;
    seq_t val = ((seq_t)11 * nn * nn - (seq_t)7 * nn) / (seq_t)2;
    return val;
}
