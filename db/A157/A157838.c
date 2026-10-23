/*
 * A157838  a(n) = 3600*n^2 - 6049*n + 2541.
 * https://oeis.org/A157838
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3600*n^2 - 6049*n + 2541
 * Range:     u8 n <= 1, u16 n <= 5, u32 n <= 1093, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;
    return 3600 * nn * nn - 6049 * nn + 2541;
}
