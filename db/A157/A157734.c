/*
 * A157734  a(n) = 441*n^2 - 394*n + 88.
 * https://oeis.org/A157734
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 441*n^2 - 394*n + 88
 * Range:     u8 n <= 1, u16 n <= 12, u32 n <= 3121, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 0;             /* offset starts at 1 */
    seq_t nn = (seq_t)n;
    return 441 * nn * nn - 394 * nn + 88;
}
