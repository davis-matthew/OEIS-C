/*
 * A157368  a(n) = 49*n^2 - 78*n + 31.
 * https://oeis.org/A157368
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 49*n^2 - 78*n + 31
 * Range:     u8 n <= 3, u16 n <= 37, u32 n <= 9363, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t tn = (seq_t)n;
    return 49*tn*tn - 78*tn + 31;
}
