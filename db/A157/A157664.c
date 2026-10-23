/*
 * A157664  a(n) = 80000*n^2 + 800*n + 1.
 * https://oeis.org/A157664
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 80000*n^2 + 800*n + 1
 * Range:     u8 none, u16 none, u32 n <= 231, u64 n <= 10000, u128 n <= 10000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t tn = (seq_t)n;
    return (seq_t)80000 * tn * tn + (seq_t)800 * tn + (seq_t)1;
}
