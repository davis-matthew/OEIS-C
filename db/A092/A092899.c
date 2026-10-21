/*
 * A092899  Expansion of (1+2x+3x^2+6x^3)/((1+x)(1-x)^2).
 * https://oeis.org/A092899
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(0) = 1; for n > 0, a(n) = 6*n - 4 - (-1)^n (i.e. a(n) = 6*n -
 *            5 if n even, 6*n - 3 if n odd)
 * Range:     u8 n <= 43, u16 n <= 56, u32 n <= 56, u64 n <= 56, u128 n <= 56,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == (idx_t)0)
        return (seq_t)1;
    seq_t six_n = (seq_t)6 * n;
    if ((n & (idx_t)1) == 0) /* n even */
        return six_n - (seq_t)5;
    else                       /* n odd */
        return six_n - (seq_t)3;
}
