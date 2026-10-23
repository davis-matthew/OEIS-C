/*
 * A176592  List of pairs n, 11*n.
 * https://oeis.org/A176592
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf, easy
 * Formula:   a(n) = (n==1?1:(n%2==0?11*n/2:(n+1)/2))
 * Range:     u8 n <= 47, u16 n <= 64, u32 n <= 64, u64 n <= 64, u128 n <= 64,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 1u)
        return (seq_t)1;

    seq_t nt = (seq_t)n;
    if ((nt & (seq_t)1) == 0)          /* even n */
        return (nt * (seq_t)11) / (seq_t)2;
    else                               /* odd n > 1 */
        return (nt + (seq_t)1) / (seq_t)2;
}
