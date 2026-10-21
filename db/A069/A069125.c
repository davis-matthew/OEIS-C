/*
 * A069125  a(n) = (11*n^2 - 11*n + 2)/2.
 * https://oeis.org/A069125
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   a(n) = (11*n^2 - 11*n + 2)/2 (offset 1)
 * Range:     u8 n <= 7, u16 n <= 109, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 0;               /* undefined for n=0, return 0 */
    seq_t ns = (seq_t)n;
    seq_t nn = ns - 1;                   /* n*(n-1) is always even */
    seq_t prod = ns * nn;                /* even intermediate product   */
    seq_t half = prod / 2;               /* divide by 2 safely           */
    seq_t res = (half * (seq_t)11) + (seq_t)1;
    return res;
}
