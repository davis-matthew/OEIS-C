/*
 * A063494  a(n) = (2*n - 1)*(7*n^2 - 7*n + 3)/3.
 * https://oeis.org/A063494
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (2*n - 1)*(7*n^2 - 7*n + 3)/3
 * Range:     u8 n <= 4, u16 n <= 24, u32 n <= 734, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn   = (seq_t)n;
    seq_t term1 = 2 * nn - 1;
    seq_t term2 = 7 * nn * nn - 7 * nn + 3;
    return (term1 * term2) / 3;
}
