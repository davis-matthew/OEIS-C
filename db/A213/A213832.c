/*
 * A213832  Principal diagonal of the convolution array A213831.
 * https://oeis.org/A213832
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n - 7*n^2 + 8*n^3)/2
 * Range:     u8 n <= 4, u16 n <= 25, u32 n <= 900, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nt = (seq_t)n;
    seq_t t1 = nt * nt;        /* n^2 */
    seq_t t2 = t1 * nt;        /* n^3 */
    seq_t val = (nt - (seq_t)7 * t1 + (seq_t)8 * t2) / 2;
    return val;
}
