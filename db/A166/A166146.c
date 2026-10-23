/*
 * A166146  a(n) = (7*n^2 + 7*n - 12)/2.
 * https://oeis.org/A166146
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (7*n^2 + 7*n - 12)/2
 * Range:     u8 n <= 6, u16 n <= 135, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn   = n;
    seq_t prod = (seq_t)7 * nn * nn;
    seq_t add  = (seq_t)7 * nn;
    return (prod + add - (seq_t)12) / (seq_t)2;
}
