/*
 * A066443  Number of distinct paths of length 2n+1 along edges of a unit cube
 *          between two fixed adjacent vertices.
 * https://oeis.org/A066443
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, changed
 * Formula:   a(n) = (3^(2*n+1)+1)/4. It is equivalent to the linear
 *            recurrence a(0) = 1, a(n) = 9*a(n-1) - 2 for n ≥ 1. This
 *            implementation uses the recurrence, which requires only integer
 *            multiplication by the small constant 9 and subtraction of 2, and
 *            works for all supported seq_t sizes
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 10, u64 n <= 20, u128 n <= 40,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 1;
    seq_t prev = 1;               /* a(0) */
    for (idx_t i = 1; i <= n; ++i)
    {
        prev = prev * 9 - 2;      /* a(i) = 9*a(i-1) - 2 */
    }
    return prev;
}
