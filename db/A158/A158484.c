/*
 * A158484  a(n) = 49*n^2 - 7.
 * https://oeis.org/A158484
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 49*n^2 - 7
 * Range:     u8 n <= 2, u16 n <= 36, u32 n <= 9362, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* undefined for n < 1, return 0 for safety */
    seq_t nn = (seq_t)n;
    return 49 * nn * nn - 7;
}
