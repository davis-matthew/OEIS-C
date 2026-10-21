/*
 * A086514  Difference between the arithmetic mean of the neighbors of the
 *          terms and the term itself follows the pattern 0,1,2,3,4,5,...
 * https://oeis.org/A086514
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n^3-6*n^2+14*n-6)/3
 * Range:     u8 n <= 8, u16 n <= 42, u32 n <= 2089, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;
    seq_t t  = nn * nn * nn - 6 * nn * nn + 14 * nn - 6;
    return t / 3;
}
