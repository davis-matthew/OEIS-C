/*
 * A228741  Number of arrays of the median of three adjacent elements of some
 *          length-6 0..n array.
 * https://oeis.org/A228741
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (n+1)*(2*n^3+10*n^2+9*n+3)/3
 * Range:     u8 n <= 3, u16 n <= 16, u32 n <= 210, u64 n <= 210,
 *            u128 n <= 210, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* undefined for n < 1, but safe */
    seq_t sn = (seq_t)n;
    seq_t n2 = sn * sn;                 /* n^2 */
    seq_t n3 = n2 * sn;                 /* n^3 */
    seq_t num = (n3 * 2) + (n2 * 10) + (sn * 9) + 3;
    seq_t val = (sn + 1) * num / 3;
    return val;
}
