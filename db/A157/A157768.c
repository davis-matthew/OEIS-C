/*
 * A157768  a(n) = 27225*n^2 - 39202*n + 14112.
 * https://oeis.org/A157768
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 27225*n^2 - 39202*n + 14112
 * Range:     u8 none, u16 n <= 2, u32 n <= 397, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;
    return 27225 * nn * nn - 39202 * nn + 14112;
}
