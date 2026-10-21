/*
 * A236257  a(n) = 2*n^2 - 7*n + 9.
 * https://oeis.org/A236257
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*n^2 - 7*n + 9
 * Range:     u8 n <= 12, u16 n <= 182, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;
    return 2 * nn * nn - 7 * nn + 9;
}
