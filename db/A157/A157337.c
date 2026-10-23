/*
 * A157337  a(n) = 128*n^2 + 32*n + 1.
 * https://oeis.org/A157337
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 128*n*n + 32*n + 1
 * Range:     u8 n <= 1, u16 n <= 22, u32 n <= 5792, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;
    return 128 * nn * nn + 32 * nn + 1;
}
