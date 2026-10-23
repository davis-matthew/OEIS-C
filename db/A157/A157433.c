/*
 * A157433  a(n) = 128n^2 + 2336n + 10657.
 * https://oeis.org/A157433
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 128*n^2 + 2336*n + 10657
 * Range:     u8 none, u16 n <= 13, u32 n <= 5783, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;
    return 128 * nn * nn + 2336 * nn + 10657;
}
