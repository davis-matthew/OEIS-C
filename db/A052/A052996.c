/*
 * A052996  Expansion of (1+x^2-x^3)/((1-x)*(1-2*x)).
 * https://oeis.org/A052996
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(0)=1, a(1)=3, a(2)=8; for n>1: a(n)=9*2^(n-2)-1
 * Range:     u8 n <= 6, u16 n <= 14, u32 n <= 30, u64 n <= 62, u128 n <= 126,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;
    if (n == 1)
        return (seq_t)3;

    /* n>=2 */
    seq_t two = (seq_t)1;
    two <<= (n - 2);           /* compute 2^(n-2) */
    return ((seq_t)9 * two) - (seq_t)1;
}
