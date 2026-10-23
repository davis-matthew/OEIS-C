/*
 * A157442  a(n) = 14641*n^2 - 24684*n + 10405.
 * https://oeis.org/A157442
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 14641*n^2 - 24684*n + 10405
 * Range:     u8 none, u16 n <= 2, u32 n <= 542, u64 n <= 1000, u128 n <= 1000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;
    return 14641 * nn * nn - 24684 * nn + 10405;
}
