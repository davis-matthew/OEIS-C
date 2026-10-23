/*
 * A157264  a(n) = 10368*n^2 - 15840*n + 6049.
 * https://oeis.org/A157264
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 10368*n*n - 15840*n + 6049
 * Range:     u8 none, u16 n <= 3, u32 n <= 644, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t tn = (seq_t)n;
    return 10368 * tn * tn - 15840 * tn + 6049;
}
